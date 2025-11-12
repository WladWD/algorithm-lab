file(GLOB SRC src/*.cpp)
add_library(data_structures_range_query_mo STATIC ${SRC})
target_include_directories(data_structures_range_query_mo PUBLIC include)

add_library(data_structures::range_query::mo ALIAS data_structures_range_query_mo)
target_compile_features(data_structures_range_query_mo PUBLIC cxx_std_23)
