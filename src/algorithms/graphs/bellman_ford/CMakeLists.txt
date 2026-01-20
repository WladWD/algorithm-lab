# ---- Bellman–Ford module ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_bellman_ford STATIC ${SRC})
target_include_directories(algo_graphs_bellman_ford PUBLIC include)

add_library(algo::graphs::bellman_ford ALIAS algo_graphs_bellman_ford)
target_compile_features(algo_graphs_bellman_ford PUBLIC cxx_std_23)
