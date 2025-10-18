# ---- Dijkstra module  ----
add_library(algo_graphs_dijkstra STATIC src)
target_include_directories(algo_graphs_dijkstra PUBLIC include)

add_library(algo::graphs::dijkstra ALIAS algo_graphs_dijkstra)
target_compile_features(algo_graphs_dijkstra PUBLIC cxx_std_23)
