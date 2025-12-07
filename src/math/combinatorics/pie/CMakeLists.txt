file(GLOB SRC src/*.cpp)
add_library(algebra_pie STATIC ${SRC})
target_include_directories(algebra_pie PUBLIC include)
add_library(algebra::pie ALIAS algebra_pie)
target_compile_features(algebra_pie PUBLIC cxx_std_23)

