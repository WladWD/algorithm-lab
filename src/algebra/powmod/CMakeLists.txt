file(GLOB SRC src/*.cpp)
add_library(algo_algebra_powmod STATIC ${SRC})
target_include_directories(algo_algebra_powmod PUBLIC include)

add_library(algo::algebra::powmod ALIAS algo_algebra_powmod)
target_compile_features(algo_algebra_powmod PUBLIC cxx_std_23)
