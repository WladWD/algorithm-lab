# ---- Rolling Hash module ----
file(GLOB SRC src/*.cpp)
add_library(algo_strings_rolling_hash STATIC ${SRC})
target_include_directories(algo_strings_rolling_hash PUBLIC include)

add_library(algo::strings::rolling_hash ALIAS algo_strings_rolling_hash)
target_compile_features(algo_strings_rolling_hash PUBLIC cxx_std_23)

