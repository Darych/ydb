UNITTEST_FOR(ydb/core/tx/tx_allocator_client)

OWNER(
    svc
    g:kikimr
)

FORK_SUBTESTS()

TIMEOUT(600)

SIZE(MEDIUM)

PEERDIR(
    library/cpp/testing/unittest
    ydb/core/mind
    ydb/core/testlib
    ydb/core/tx
    ydb/core/tx/tx_allocator_client
)

YQL_LAST_ABI_VERSION()

SRCS(
    actor_client_ut.cpp
    ut_helpers.cpp
)

END()
