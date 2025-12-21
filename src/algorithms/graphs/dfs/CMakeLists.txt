# ---- DFS module ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_dfs STATIC ${SRC})
target_include_directories(algo_graphs_dfs PUBLIC include)

add_library(algo::graphs::dfs ALIAS algo_graphs_dfs)
target_compile_features(algo_graphs_dfs PUBLIC cxx_std_23)

