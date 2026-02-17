# ---- Edmonds-Karp module ----
file(GLOB SRC src/*.cpp)
add_library(algo_graphs_edmonds_karp STATIC ${SRC})
target_include_directories(algo_graphs_edmonds_karp PUBLIC include)

add_library(algo::graphs::edmonds_karp ALIAS algo_graphs_edmonds_karp)
target_compile_features(algo_graphs_edmonds_karp PUBLIC cxx_std_23)

