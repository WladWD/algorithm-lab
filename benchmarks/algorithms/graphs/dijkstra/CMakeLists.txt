    add_executable(bench_graphs_dijkstra bench_dijkstra.cpp)
    target_link_libraries(bench_graphs_dijkstra PRIVATE
            algo_graphs_dijkstra
            benchmark::benchmark
            benchmark::benchmark_main
    )