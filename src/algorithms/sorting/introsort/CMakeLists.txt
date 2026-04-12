# ---- IntroSort module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_introsort STATIC ${SRC})
target_include_directories(algo_sorting_introsort PUBLIC include)

add_library(algo::sorting::introsort ALIAS algo_sorting_introsort)
target_compile_features(algo_sorting_introsort PUBLIC cxx_std_23)

