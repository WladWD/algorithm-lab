add_executable(test_graphs_dijkstra dijkstra/test_dijkstra.cpp)
target_link_libraries(test_graphs_dijkstra PRIVATE
        algo_graphs_dijkstra
        GTest::gtest_main
)
add_test(NAME graphs.dijkstra COMMAND test_graphs_dijkstra)

add_executable(test_graphs_bipartite bipartite_check/test_bipartite_check.cpp)
target_link_libraries(test_graphs_bipartite PRIVATE
        algo_graphs_bipartite_check
        GTest::gtest_main
)
add_test(NAME graphs.bipartite COMMAND test_graphs_bipartite)

add_executable(test_graphs_dfs dfs/test_dfs.cpp)
target_link_libraries(test_graphs_dfs PRIVATE
        algo_graphs_dfs
        GTest::gtest_main
)
add_test(NAME graphs.dfs COMMAND test_graphs_dfs)
