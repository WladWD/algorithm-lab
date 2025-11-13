file(GLOB SRC src/*.cpp)
add_library(algo_algebra_gcd STATIC ${SRC})
target_include_directories(algo_algebra_gcd PUBLIC include)

add_library(algo::algebra::gcd ALIAS algo_algebra_gcd)
target_compile_features(algo_algebra_gcd PUBLIC cxx_std_23)
