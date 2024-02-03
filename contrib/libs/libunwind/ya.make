# Generated by devtools/yamaker from nixpkgs 22.11.

LIBRARY()

LICENSE(
    Apache-2.0 AND
    Apache-2.0 WITH LLVM-exception AND
    MIT AND
    NCSA
)

LICENSE_TEXTS(.yandex_meta/licenses.list.txt)

VERSION(18.1.0-rc1)

ORIGINAL_SOURCE(https://github.com/llvm/llvm-project/archive/llvmorg-18.1.0-rc1.tar.gz)

DISABLE(USE_LTO)

PEERDIR(
    library/cpp/sanitizer/include
)

ADDINCL(
    contrib/libs/libunwind/include
)

NO_RUNTIME()

NO_UTIL()

NO_SANITIZE()

NO_SANITIZE_COVERAGE()

CFLAGS(
    GLOBAL -D_libunwind_
    -D_LIBUNWIND_IS_NATIVE_ONLY
    -fno-exceptions
    -fno-rtti
    -funwind-tables
)

IF (SANITIZER_TYPE == memory)
    CFLAGS(
        -fPIC
    )
ENDIF()

SRCS(
    src/Unwind-EHABI.cpp
    src/Unwind-seh.cpp
    src/Unwind-sjlj.c
    src/Unwind-wasm.c
    src/UnwindLevel1-gcc-ext.c
    src/UnwindLevel1.c
    src/UnwindRegistersRestore.S
    src/UnwindRegistersSave.S
    src/libunwind.cpp
)

END()
