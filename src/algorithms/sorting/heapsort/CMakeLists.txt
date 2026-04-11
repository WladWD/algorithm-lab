# ---- HeapSort module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_heapsort STATIC ${SRC})
target_include_directories(algo_sorting_heapsort PUBLIC include)

add_library(algo::sorting::heapsort ALIAS algo_sorting_heapsort)
target_compile_features(algo_sorting_heapsort PUBLIC cxx_std_23)

