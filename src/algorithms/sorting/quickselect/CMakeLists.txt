# ---- QuickSelect module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_quickselect STATIC ${SRC})
target_include_directories(algo_sorting_quickselect PUBLIC include)

add_library(algo::sorting::quickselect ALIAS algo_sorting_quickselect)
target_compile_features(algo_sorting_quickselect PUBLIC cxx_std_23)

