file(GLOB SRC src/*.cpp)
add_library(algo_algebra_mod_inv STATIC ${SRC})
target_include_directories(algo_algebra_mod_inv PUBLIC include)

add_library(algo::algebra::mod_inv ALIAS algo_algebra_mod_inv)
target_compile_features(algo_algebra_mod_inv PUBLIC cxx_std_23)
