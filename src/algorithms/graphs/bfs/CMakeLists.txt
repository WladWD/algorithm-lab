# ---- BFS module ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_bfs STATIC ${SRC})
target_include_directories(algo_graphs_bfs PUBLIC include)

add_library(algo::graphs::bfs ALIAS algo_graphs_bfs)
target_compile_features(algo_graphs_bfs PUBLIC cxx_std_23)

