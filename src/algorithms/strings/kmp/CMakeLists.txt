# ---- KMP module ----
file(GLOB SRC src/*.cpp)
add_library(algo_strings_kmp STATIC ${SRC})
target_include_directories(algo_strings_kmp PUBLIC include)

add_library(algo::strings::kmp ALIAS algo_strings_kmp)
target_compile_features(algo_strings_kmp PUBLIC cxx_std_23)

