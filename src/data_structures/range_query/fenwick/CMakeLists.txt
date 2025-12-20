file(GLOB SRC src/*.cpp)
add_library(data_structures_range_query_fenwick STATIC ${SRC})
target_include_directories(data_structures_range_query_fenwick PUBLIC include)

add_library(data_structures::range_query::fenwick ALIAS data_structures_range_query_fenwick)
target_compile_features(data_structures_range_query_fenwick PUBLIC cxx_std_23)

