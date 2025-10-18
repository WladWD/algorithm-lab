option(ALGO_ENABLE_ALGORITHMS_GRAPHS_DIJKSTRA_BENCH "Build Dijkstra algorithm bench" ON)

if (ALGO_ENABLE_ALGORITHMS_GRAPHS_DIJKSTRA_BENCH)
    add_subdirectory(algorithms/graphs/dijkstra)
endif()