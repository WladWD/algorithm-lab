add_executable(test_graphs_dijkstra test_dijkstra.cpp)
target_link_libraries(test_graphs_dijkstra PRIVATE
        algo_graphs_dijkstra
        GTest::gtest_main
)
add_test(NAME graphs.dijkstra COMMAND test_graphs_dijkstra)