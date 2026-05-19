# ---- Rabin-Karp module ----
file(GLOB SRC src/*.cpp)
add_library(algo_strings_rabin_karp STATIC ${SRC})
target_include_directories(algo_strings_rabin_karp PUBLIC include)

add_library(algo::strings::rabin_karp ALIAS algo_strings_rabin_karp)
target_compile_features(algo_strings_rabin_karp PUBLIC cxx_std_23)
