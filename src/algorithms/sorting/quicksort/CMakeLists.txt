# ---- QuickSort module  ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_quicksort STATIC ${SRC})
target_include_directories(algo_sorting_quicksort PUBLIC include)

add_library(algo::sorting::quicksort ALIAS algo_sorting_quicksort)
target_compile_features(algo_sorting_quicksort PUBLIC cxx_std_23)

