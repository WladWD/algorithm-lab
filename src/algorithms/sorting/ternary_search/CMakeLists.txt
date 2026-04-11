# ---- Ternary Search module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_ternary_search STATIC ${SRC})
target_include_directories(algo_sorting_ternary_search PUBLIC include)

add_library(algo::sorting::ternary_search ALIAS algo_sorting_ternary_search)
target_compile_features(algo_sorting_ternary_search PUBLIC cxx_std_23)

