# ---- Topological Sort module ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_topological_sort STATIC ${SRC})
target_include_directories(algo_graphs_topological_sort PUBLIC include)

add_library(algo::graphs::topological_sort ALIAS algo_graphs_topological_sort)
target_compile_features(algo_graphs_topological_sort PUBLIC cxx_std_23)

