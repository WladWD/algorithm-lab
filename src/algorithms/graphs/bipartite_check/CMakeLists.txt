file(GLOB SRC src/*.cpp)
add_library(algo_graphs_bipartite_check STATIC ${SRC})
target_include_directories(algo_graphs_bipartite_check PUBLIC include)

add_library(algo::graphs::bipartite_check ALIAS algo_graphs_bipartite_check)
target_compile_features(algo_graphs_bipartite_check PUBLIC cxx_std_23)
