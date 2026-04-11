# ---- Counting Sort module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_counting_sort STATIC ${SRC})
target_include_directories(algo_sorting_counting_sort PUBLIC include)

add_library(algo::sorting::counting_sort ALIAS algo_sorting_counting_sort)
target_compile_features(algo_sorting_counting_sort PUBLIC cxx_std_23)

