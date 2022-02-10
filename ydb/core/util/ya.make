LIBRARY()

OWNER(
    ddoarn
    g:kikimr
)

SRCS(
    address_classifier.cpp
    cache.cpp
    cache.h
    cache_cache.h
    circular_queue.h
    concurrent_rw_hash.cpp
    concurrent_rw_hash.h
    console.cpp
    console.h
    counted_leaky_bucket.h
    defs.h
    failure_injection.cpp
    failure_injection.h
    fast_tls.cpp
    format.cpp
    format.h
    fragmented_buffer.cpp 
    fragmented_buffer.h 
    hazard.cpp
    hyperlog_counter.cpp
    hyperlog_counter.h
    interval_set.h 
    intrusive_fixed_hash_set.h
    intrusive_heap.cpp
    intrusive_heap.h
    intrusive_stack.h 
    log_priority_mute_checker.h
    memory_tracker.cpp
    memory_tracker.h
    operation_queue.h
    page_map.cpp
    pb.h
    proto_duration.h
    queue_inplace.h
    queue_oneone_inplace.h
    simple_cache.h
    single_thread_ic_mock.cpp
    single_thread_ic_mock.h
    stlog.h
    templates.h
    testactorsys.cpp
    testactorsys.h
    text.cpp 
    text.h 
    time_series_vec.h
    token_bucket.h
    throughput_meter.h
    tuples.h
    type_alias.h
    ulid.cpp
    ulid.h
    wildcard.h
    yverify_stream.h
)

PEERDIR(
    library/cpp/actors/interconnect/mock
    library/cpp/actors/util
    library/cpp/containers/stack_vector
    library/cpp/html/escape
    library/cpp/ipmath
    library/cpp/lwtrace
    library/cpp/monlib/dynamic_counters
    library/cpp/random_provider
    ydb/core/base
    ydb/core/protos
)

END()

RECURSE_FOR_TESTS(
    btree_benchmark
    ut
)
