# ---- MergeSort module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_mergesort STATIC ${SRC})
target_include_directories(algo_sorting_mergesort PUBLIC include)

add_library(algo::sorting::mergesort ALIAS algo_sorting_mergesort)
target_compile_features(algo_sorting_mergesort PUBLIC cxx_std_23)

