# ---- Dinic module ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_dinic STATIC ${SRC})
target_include_directories(algo_graphs_dinic PUBLIC include)

add_library(algo::graphs::dinic ALIAS algo_graphs_dinic)
target_compile_features(algo_graphs_dinic PUBLIC cxx_std_23)

