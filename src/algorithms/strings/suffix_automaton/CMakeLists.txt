# ---- Suffix Automaton module ----
file(GLOB SRC src/*.cpp)
add_library(algo_strings_suffix_automaton STATIC ${SRC})
target_include_directories(algo_strings_suffix_automaton PUBLIC include)

add_library(algo::strings::suffix_automaton ALIAS algo_strings_suffix_automaton)
target_compile_features(algo_strings_suffix_automaton PUBLIC cxx_std_23)
