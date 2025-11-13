file(GLOB SRC src/*.cpp)
add_library(algo_algebra_gcdext STATIC ${SRC})
target_include_directories(algo_algebra_gcdext PUBLIC include)

add_library(algo::algebra::gcdext ALIAS algo_algebra_gcdext)
target_compile_features(algo_algebra_gcdext PUBLIC cxx_std_23)
