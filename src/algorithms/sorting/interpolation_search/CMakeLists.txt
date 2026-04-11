# ---- Interpolation Search module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_interpolation_search STATIC ${SRC})
target_include_directories(algo_sorting_interpolation_search PUBLIC include)

add_library(algo::sorting::interpolation_search ALIAS algo_sorting_interpolation_search)
target_compile_features(algo_sorting_interpolation_search PUBLIC cxx_std_23)

