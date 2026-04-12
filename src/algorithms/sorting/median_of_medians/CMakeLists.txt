# ---- Median of Medians module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_median_of_medians STATIC ${SRC})
target_include_directories(algo_sorting_median_of_medians PUBLIC include)

add_library(algo::sorting::median_of_medians ALIAS algo_sorting_median_of_medians)
target_compile_features(algo_sorting_median_of_medians PUBLIC cxx_std_23)

