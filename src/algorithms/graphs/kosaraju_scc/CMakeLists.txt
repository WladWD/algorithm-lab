# ---- Kosaraju SCC module ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_kosaraju_scc STATIC ${SRC})
target_include_directories(algo_graphs_kosaraju_scc PUBLIC include)

add_library(algo::graphs::kosaraju_scc ALIAS algo_graphs_kosaraju_scc)
target_compile_features(algo_graphs_kosaraju_scc PUBLIC cxx_std_23)
