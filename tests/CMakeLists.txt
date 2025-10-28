option(ALGO_ENABLE_ALGORITHMS_TESTS "Build algorithms tests" ON)
option(ALGO_ENABLE_DATA_STRUCTURES_TESTS "Build data structures tests" ON)

if (ALGO_ENABLE_ALGORITHMS_TESTS)
    add_subdirectory(algorithms/graphs)
endif()

if (ALGO_ENABLE_DATA_STRUCTURES_TESTS)
    add_subdirectory(data_structures/lock_free)
endif()
