#pragma once

#include "ydb_command.h"
#include "ydb_common.h"

#include <ydb/public/sdk/cpp/client/ydb_scheme/scheme.h>
#include <ydb/public/sdk/cpp/client/ydb_table/table.h>
#include <ydb/public/lib/ydb_cli/common/format.h>
#include <ydb/public/lib/ydb_cli/common/pretty_table.h>
#include <ydb/public/sdk/cpp/client/ydb_persqueue_public/persqueue.h>

namespace NYdb {
namespace NConsoleClient {

class TCommandScheme : public TClientCommandTree {
public:
    TCommandScheme();
};

class TCommandMakeDirectory : public TYdbOperationCommand, public TCommandWithPath {
public:
    TCommandMakeDirectory();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;
};

class TCommandRemoveDirectory : public TYdbOperationCommand, public TCommandWithPath {
public:
    TCommandRemoveDirectory();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;
};

class TCommandDescribe : public TYdbOperationCommand, public TCommandWithPath, public TCommandWithFormat {
public:
    TCommandDescribe();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;

private:
    int PrintPathResponse(TDriver& driver, const NScheme::TDescribePathResult& result);
    int DescribeTable(TDriver& driver);
    int PrintTableResponse(NTable::TDescribeTableResult& result);
    void PrintResponsePretty(const NTable::TTableDescription& tableDescription);
    int PrintResponseProtoJsonBase64(const NTable::TTableDescription& tableDescription);
    void WarnAboutTableOptions();

    int DescribeStream(TDriver& driver); 
    int PrintStreamResponse(const NYdb::NPersQueue::TDescribeTopicResult& result); 
    int PrintStreamResponsePretty(const NYdb::NPersQueue::TDescribeTopicResult::TTopicSettings& settings); 
    int PrintStreamResponseProtoJsonBase64(const NYdb::NPersQueue::TDescribeTopicResult& result); 
 
    // Common options
    bool ShowPermissions = false;
    // Table options
    bool ShowKeyShardBoundaries = false;
    bool ShowTableStats = false;
    bool ShowPartitionStats = false;
};

class TCommandList : public TYdbOperationCommand, public TCommandWithPath {
public:
    TCommandList();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;

private:
    void PrintResponse(NScheme::TListDirectoryResult& result, const TString& path, NScheme::TSchemeClient& client);
    void PrintResponseAdvanced(NScheme::TListDirectoryResult& result, TDriver& driver);
    void AddEntriesRecursive(
        const TString& path,
        TVector<NScheme::TSchemeEntry> entries,
        size_t depth,
        TPrettyTable& table,
        bool oneExactEntry,
        NTable::TTableClient& tableClient,
        NScheme::TSchemeClient& schemeClient
    );

    bool AdvancedMode = false;
    bool Recursive = false;
};

class TCommandPermissions : public TClientCommandTree {
public:
    TCommandPermissions();
};

class TCommandPermissionGrant : public TYdbOperationCommand, public TCommandWithPath {
public:
    TCommandPermissionGrant();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;

private:
    TString Subject;
    TVector<TString> PermissionsToGrant;
};

class TCommandPermissionRevoke : public TYdbOperationCommand, public TCommandWithPath {
public:
    TCommandPermissionRevoke();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;

private:
    TString Subject;
    TVector<TString> PermissionsToRevoke;
};

class TCommandPermissionSet : public TYdbOperationCommand, public TCommandWithPath {
public:
    TCommandPermissionSet();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;

private:
    TString Subject;
    TVector<TString> PermissionsToSet;
};

class TCommandChangeOwner : public TYdbOperationCommand, public TCommandWithPath {
public:
    TCommandChangeOwner();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;

private:
    TString Owner;
};

class TCommandPermissionClear : public TYdbOperationCommand, public TCommandWithPath {
public:
    TCommandPermissionClear();
    virtual void Config(TConfig& config) override;
    virtual void Parse(TConfig& config) override;
    virtual int Run(TConfig& config) override;
};

}
}
