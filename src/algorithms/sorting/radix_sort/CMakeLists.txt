# ---- Radix Sort module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_radix_sort STATIC ${SRC})
target_include_directories(algo_sorting_radix_sort PUBLIC include)

add_library(algo::sorting::radix_sort ALIAS algo_sorting_radix_sort)
target_compile_features(algo_sorting_radix_sort PUBLIC cxx_std_23)

