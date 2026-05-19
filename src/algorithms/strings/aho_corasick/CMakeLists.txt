# ---- Aho-Corasick module ----
file(GLOB SRC src/*.cpp)
add_library(algo_strings_aho_corasick STATIC ${SRC})
target_include_directories(algo_strings_aho_corasick PUBLIC include)

add_library(algo::strings::aho_corasick ALIAS algo_strings_aho_corasick)
target_compile_features(algo_strings_aho_corasick PUBLIC cxx_std_23)

