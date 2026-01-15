file(GLOB SRC src/*.cpp)
add_library(algo_algebra_sieve STATIC ${SRC})
target_include_directories(algo_algebra_sieve PUBLIC include)

target_compile_features(algo_algebra_sieve PUBLIC cxx_std_23)
add_library(algo::algebra::sieve ALIAS algo_algebra_sieve)
