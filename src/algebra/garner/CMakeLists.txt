file(GLOB SRC src/*.cpp)
add_library(algo_algebra_garner STATIC ${SRC})
target_include_directories(algo_algebra_garner PUBLIC include)

add_library(algo::algebra::garner ALIAS algo_algebra_garner)
target_compile_features(algo_algebra_garner PUBLIC cxx_std_23)
