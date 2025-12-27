file(GLOB SRC src/*.cpp)
add_library(data_structures_dsu STATIC ${SRC})
target_include_directories(data_structures_dsu PUBLIC include)

target_compile_features(data_structures_dsu PUBLIC cxx_std_23)
add_library(data_structures::dsu ALIAS data_structures_dsu)

