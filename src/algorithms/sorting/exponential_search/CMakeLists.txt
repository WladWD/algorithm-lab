# ---- Exponential Search module ----
file(GLOB SRC src/*.cpp)
add_library(algo_sorting_exponential_search STATIC ${SRC})
target_include_directories(algo_sorting_exponential_search PUBLIC include)

add_library(algo::sorting::exponential_search ALIAS algo_sorting_exponential_search)
target_compile_features(algo_sorting_exponential_search PUBLIC cxx_std_23)

