# ---- Suffix Array + LCP module ----
file(GLOB SRC src/*.cpp)
add_library(algo_strings_suffix_array STATIC ${SRC})
target_include_directories(algo_strings_suffix_array PUBLIC include)

add_library(algo::strings::suffix_array ALIAS algo_strings_suffix_array)
target_compile_features(algo_strings_suffix_array PUBLIC cxx_std_23)
