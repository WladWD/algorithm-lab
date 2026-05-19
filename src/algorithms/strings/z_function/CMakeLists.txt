# ---- Z-Function module ----
file(GLOB SRC src/*.cpp)
add_library(algo_strings_z_function STATIC ${SRC})
target_include_directories(algo_strings_z_function PUBLIC include)

add_library(algo::strings::z_function ALIAS algo_strings_z_function)
target_compile_features(algo_strings_z_function PUBLIC cxx_std_23)

