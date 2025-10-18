option(ALGO_ENABLE_ALGORITHMS_GRAPHS_DIJKSTRA_TEST "Build Dijkstra algorithm tests" ON)

if (ALGO_ENABLE_ALGORITHMS_GRAPHS_DIJKSTRA_TEST)
    add_subdirectory(algorithms/graphs/dijkstra)
endif()