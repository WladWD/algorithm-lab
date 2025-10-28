file(GLOB_RECURSE TEST_SRC *.cpp)

add_executable(test_algorithms_graphs ${TEST_SRC})
target_link_libraries(test_algorithms_graphs PRIVATE
        algo_graphs_dijkstra
        algo_graphs_bipartite_check
        GTest::gtest_main
)
add_test(NAME algorithms_graphs COMMAND test_algorithms_graphs)