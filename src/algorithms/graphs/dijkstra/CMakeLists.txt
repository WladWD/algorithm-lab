# ---- Dijkstra module  ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_dijkstra STATIC ${SRC})
target_include_directories(algo_graphs_dijkstra PUBLIC include)

add_library(algo::graphs::dijkstra ALIAS algo_graphs_dijkstra)
target_compile_features(algo_graphs_dijkstra PUBLIC cxx_std_23)
