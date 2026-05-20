# ---- Suffix Tree (Ukkonen) module ----
file(GLOB SRC src/*.cpp)
add_library(algo_strings_suffix_tree STATIC ${SRC})
target_include_directories(algo_strings_suffix_tree PUBLIC include)

add_library(algo::strings::suffix_tree ALIAS algo_strings_suffix_tree)
target_compile_features(algo_strings_suffix_tree PUBLIC cxx_std_23)
