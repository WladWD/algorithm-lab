# ---- Prim MST module ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_prim_mst STATIC ${SRC})
target_include_directories(algo_graphs_prim_mst PUBLIC include)

add_library(algo::graphs::prim_mst ALIAS algo_graphs_prim_mst)
target_compile_features(algo_graphs_prim_mst PUBLIC cxx_std_23)
