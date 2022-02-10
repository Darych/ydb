LIBRARY()

OWNER(
    ddoarn
    xenoxeno
    g:kikimr
)

SRCS(
    configured_tablet_bootstrapper.cpp
    configured_tablet_bootstrapper.h
    defs.h 
    dynamic_nameserver.cpp
    dynamic_nameserver.h
    dynamic_nameserver_impl.h
    dynamic_nameserver_mon.cpp
    labels_maintainer.cpp
    labels_maintainer.h
    lease_holder.cpp
    lease_holder.h
    local.cpp
    local.h
    node_broker.cpp
    node_broker.h
    node_broker_impl.h
    node_broker__extend_lease.cpp
    node_broker__init_scheme.cpp
    node_broker__load_state.cpp
    node_broker__register_node.cpp
    node_broker__scheme.h
    node_broker__update_config.cpp
    node_broker__update_config_subscription.cpp
    node_broker__update_epoch.cpp
    table_adapter.h
    tenant_node_enumeration.cpp
    tenant_node_enumeration.h
    tenant_pool.h
    tenant_pool.cpp
    tenant_slot_broker.cpp
    tenant_slot_broker.h
    tenant_slot_broker_impl.h
    tenant_slot_broker__alter_tenant.cpp
    tenant_slot_broker__assign_free_slots.cpp
    tenant_slot_broker__check_slot_status.cpp
    tenant_slot_broker__init_scheme.cpp
    tenant_slot_broker__load_state.cpp
    tenant_slot_broker__scheme.h
    tenant_slot_broker__update_config.cpp
    tenant_slot_broker__update_pool_status.cpp
    tenant_slot_broker__update_node_location.cpp
    tenant_slot_broker__update_slot_status.cpp
)

PEERDIR(
    library/cpp/actors/core
    ydb/core/actorlib_impl
    ydb/core/base
    ydb/core/blobstorage
    ydb/core/blobstorage/crypto
    ydb/core/blobstorage/dsproxy/mock
    ydb/core/blobstorage/groupinfo
    ydb/core/blobstorage/incrhuge
    ydb/core/blobstorage/pdisk
    ydb/core/engine/minikql
    ydb/core/kesus/tablet
    ydb/core/keyvalue
    ydb/core/mind/bscontroller
    ydb/core/node_whiteboard
    ydb/core/protos
    ydb/core/sys_view/processor
    ydb/core/tablet
    ydb/core/tablet_flat
    ydb/core/test_tablet
    ydb/core/tx/replication/controller
    ydb/core/tx/scheme_board
    ydb/core/tx/scheme_cache
    ydb/core/tx/schemeshard
)

END()

RECURSE(
    address_classification
    bscontroller
    hive
)

RECURSE_FOR_TESTS(
    ut
    ut_fat
)
