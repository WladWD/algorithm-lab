option(ALGO_ENABLE_ALGORITHMS_TESTS "Build algorithms tests" ON)
option(ALGO_ENABLE_DATA_STRUCTURES_TESTS "Build data structures tests" ON)
option(ALGO_ENABLE_ALGEBRA_TESTS "Build algebra tests" ON)

if (ALGO_ENABLE_ALGORITHMS_TESTS)
    add_subdirectory(algorithms)
endif()

if (ALGO_ENABLE_DATA_STRUCTURES_TESTS)
    add_subdirectory(data_structures/lock_free)
    add_subdirectory(data_structures/range_query)
    add_subdirectory(data_structures/dsu)
    add_subdirectory(data_structures/trie)
endif()

if (ALGO_ENABLE_ALGEBRA_TESTS)
    add_subdirectory(algebra)
endif()
