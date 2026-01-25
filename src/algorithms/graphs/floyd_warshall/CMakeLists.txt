# ---- Floyd–Warshall module  ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_floyd_warshall STATIC ${SRC})
target_include_directories(algo_graphs_floyd_warshall PUBLIC include)

add_library(algo::graphs::floyd_warshall ALIAS algo_graphs_floyd_warshall)
target_compile_features(algo_graphs_floyd_warshall PUBLIC cxx_std_23)

