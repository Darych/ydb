#include "blobstorage_pdisk_ut.h"

#include "blobstorage_pdisk_abstract.h"
#include "blobstorage_pdisk_impl.h"
#include "blobstorage_pdisk_ut_env.h"

#include <ydb/core/blobstorage/crypto/default.h>
#include <ydb/core/testlib/actors/test_runtime.h>

#include <util/system/hp_timer.h>

namespace NKikimr {

Y_UNIT_TEST_SUITE(TSectorMap) {

    bool TestSectorMapPerformance(NPDisk::NSectorMap::EDiskMode diskMode, ui64 diskSizeGb, ui64 dataSizeMb, bool toFirstSector,
            bool testRead, ui32 tries, double deviationRange = 0.05, std::pair<double, double>* time = nullptr) {
        ui64 dataSize = dataSizeMb * 1024 * 1024;
        ui64 deviceSize = diskSizeGb * 1024 * 1024 * 1024;

        ui64 diskRate = toFirstSector ? 200 * 1024 * 1024 : 66 * 1024 * 1024;
        ui64 sectorsNum = deviceSize / NPDisk::NSectorMap::SECTOR_SIZE;
        ui64 sectorPos = toFirstSector ? 0 : sectorsNum - dataSize / NPDisk::NSectorMap::SECTOR_SIZE - 2;
        double timeExpected = (double)dataSize / diskRate;
        double timeSum = 0;
        for (ui32 i = 0; i < tries; ++i) {
            THPTimer timer1;
            TString data = PrepareData(dataSize);
            NPDisk::TSectorMap sectorMap(deviceSize, diskMode);
            sectorMap.ZeroInit(100);

            double timeElapsed = 0;
            if (testRead) {
                TString buf;
                buf.reserve(dataSize);
                sectorMap.Write((ui8*)data.data(), dataSize, sectorPos * NPDisk::NSectorMap::SECTOR_SIZE);
                THPTimer timer;
                sectorMap.Read((ui8*)buf.data(), dataSize, sectorPos * NPDisk::NSectorMap::SECTOR_SIZE);
                timeElapsed = timer.Passed();
            } else {
                THPTimer timer;
                sectorMap.Write((ui8*)data.data(), dataSize, sectorPos * NPDisk::NSectorMap::SECTOR_SIZE);
                timeElapsed = timer.Passed();
            }

            timeSum += timeElapsed;
        }
        double timeAvg = timeSum / tries;
        double relativeDeviation = (timeAvg - timeExpected) / timeExpected;
        if (time) {
            *time = { timeExpected, timeAvg };
        }

        return std::abs(relativeDeviation) <= deviationRange;
    }

    Y_UNIT_TEST(SectorMapPerformance) {
        std::vector<TString> failedTests;

        std::pair<double, double> time;

        using EDiskMode = NPDisk::NSectorMap::EDiskMode;
        auto test = [&](EDiskMode diskMode, ui32 diskSizeGb, ui32 dataSizeMb, bool toFirstSector, bool testRead, ui32 tries) {
            if (!TestSectorMapPerformance(diskMode, diskSizeGb, dataSizeMb, toFirstSector, testRead, tries, 0.05, &time)) {
                failedTests.push_back(TStringBuilder() << diskSizeGb << "GB " << NPDisk::NSectorMap::DiskModeToString(diskMode) <<    
                        (testRead ? " read " : " write ") << dataSizeMb << " MB to" << (toFirstSector ? " first " : " last ") << 
                        "sector, timeExpected=" << time.first << ", timeAverage=" << time.second);
            }
        };

        for (auto diskMode : { EDiskMode::DM_HDD}) {
            for (ui32 dataSizeMb : { 1, 10 }) {
                for (bool testRead : { false, true }) {
                    test(diskMode, 1960, dataSizeMb, true, testRead, 5);
                }
            }
        }
        
        test(EDiskMode::DM_HDD, 1960, 100, true, true, 3);
        test(EDiskMode::DM_HDD, 1960, 100, true, false, 3);
        test(EDiskMode::DM_HDD, 1960, 100, false, true, 3);
        test(EDiskMode::DM_HDD, 1960, 100, false, false, 3);

        for (auto& testName : failedTests) {
            Cerr << testName << Endl;
        }
        UNIT_ASSERT(failedTests.empty());
    }
}
}
