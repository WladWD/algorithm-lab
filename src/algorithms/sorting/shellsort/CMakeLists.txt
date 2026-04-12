# ---- ShellSort module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_shellsort STATIC ${SRC})
target_include_directories(algo_sorting_shellsort PUBLIC include)

add_library(algo::sorting::shellsort ALIAS algo_sorting_shellsort)
target_compile_features(algo_sorting_shellsort PUBLIC cxx_std_23)

