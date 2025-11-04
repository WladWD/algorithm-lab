option(ALGO_ENABLE_ALGORITHMS_BENCH "Build algorithms bench" ON)
option(ALGO_ENABLE_DATA_STRUCTURES_BENCH "Build data structures bench" ON)

if (ALGO_ENABLE_ALGORITHMS_BENCH)
    add_subdirectory(algorithms/graphs/dijkstra)
endif()

if (ALGO_ENABLE_DATA_STRUCTURES_BENCH)
    add_subdirectory(data_structures/lock_free/stack)
    add_subdirectory(data_structures/lock_free/queue)
endif()