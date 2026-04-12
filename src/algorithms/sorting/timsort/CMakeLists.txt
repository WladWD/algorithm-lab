# ---- TimSort module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_timsort STATIC ${SRC})
target_include_directories(algo_sorting_timsort PUBLIC include)

add_library(algo::sorting::timsort ALIAS algo_sorting_timsort)
target_compile_features(algo_sorting_timsort PUBLIC cxx_std_23)

