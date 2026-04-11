# ---- Binary Search module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_binary_search STATIC ${SRC})
target_include_directories(algo_sorting_binary_search PUBLIC include)

add_library(algo::sorting::binary_search ALIAS algo_sorting_binary_search)
target_compile_features(algo_sorting_binary_search PUBLIC cxx_std_23)

