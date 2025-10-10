# ---- Dijkstra module (library + test + bench) ----
add_library(algo_graphs_dijkstra
        dijkstra.cpp
        generator.cpp
)
target_include_directories(algo_graphs_dijkstra
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}          # so you can include "algo/graphs/dijkstra/dijkstra.h"
)
# If you’d rather use "include/" as public headers root, move headers there and adjust paths.

add_library(algo::graphs::dijkstra ALIAS algo_graphs_dijkstra)

# ---- Tests ----
if (ALGO_ENABLE_TESTS)
    add_executable(test_graphs_dijkstra test_dijkstra.cpp)
    target_link_libraries(test_graphs_dijkstra PRIVATE
            algo_graphs_dijkstra
            GTest::gtest_main
    )
    add_test(NAME graphs.dijkstra COMMAND test_graphs_dijkstra)
endif()

# ---- Benchmarks ----
if (ALGO_ENABLE_BENCH)
    add_executable(bench_graphs_dijkstra
            bench_dijkstra.cpp
    )
    target_link_libraries(bench_graphs_dijkstra PRIVATE
            algo_graphs_dijkstra
            benchmark::benchmark
            benchmark::benchmark_main
    )

endif()