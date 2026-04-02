file(GLOB SRC src/*.cpp)
add_library(algo_bit_manipulation_bit_enum STATIC ${SRC})
target_include_directories(algo_bit_manipulation_bit_enum PUBLIC include)

target_compile_features(algo_bit_manipulation_bit_enum PUBLIC cxx_std_23)
add_library(algo::bit_manipulation::bit_enum ALIAS algo_bit_manipulation_bit_enum)

