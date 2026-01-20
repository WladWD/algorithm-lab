add_executable(test_graphs_dijkstra dijkstra/test_dijkstra.cpp)
target_link_libraries(test_graphs_dijkstra PRIVATE
        algo_graphs_dijkstra
        GTest::gtest_main
)
add_test(NAME graphs.dijkstra COMMAND test_graphs_dijkstra)

add_executable(test_graphs_bellman_ford bellman_ford/test_bellman_ford.cpp)
target_link_libraries(test_graphs_bellman_ford PRIVATE
        algo_graphs_bellman_ford
        GTest::gtest_main
)
add_test(NAME graphs.bellman_ford COMMAND test_graphs_bellman_ford)

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

add_executable(test_graphs_bfs bfs/test_bfs.cpp)
target_link_libraries(test_graphs_bfs PRIVATE
        algo_graphs_bfs
        GTest::gtest_main
)
add_test(NAME graphs.bfs COMMAND test_graphs_bfs)

add_executable(test_graphs_topological_sort topological_sort/test_topological_sort.cpp)
target_link_libraries(test_graphs_topological_sort PRIVATE
        algo_graphs_topological_sort
        GTest::gtest_main
)
add_test(NAME graphs.topological_sort COMMAND test_graphs_topological_sort)

add_executable(test_graphs_kosaraju_scc kosaraju_scc/test_kosaraju_scc.cpp)
target_link_libraries(test_graphs_kosaraju_scc PRIVATE
        algo_graphs_kosaraju_scc
        GTest::gtest_main
)
add_test(NAME graphs.kosaraju_scc COMMAND test_graphs_kosaraju_scc)

add_executable(test_graphs_kruskal_mst kruskal_mst/test_kruskal_mst.cpp)
target_link_libraries(test_graphs_kruskal_mst PRIVATE
        algo_graphs_kruskal_mst
        GTest::gtest_main
)
add_test(NAME graphs.kruskal_mst COMMAND test_graphs_kruskal_mst)

add_executable(test_graphs_prim_mst prim_mst/test_prim_mst.cpp)
target_link_libraries(test_graphs_prim_mst PRIVATE
        algo_graphs_prim_mst
        GTest::gtest_main
)
add_test(NAME graphs.prim_mst COMMAND test_graphs_prim_mst)
