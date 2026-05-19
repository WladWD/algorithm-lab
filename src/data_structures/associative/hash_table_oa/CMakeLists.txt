# ---- Hash Table (open addressing) module ----
file(GLOB SRC src/*.cpp)
add_library(data_structures_hash_table_oa STATIC ${SRC})
target_include_directories(data_structures_hash_table_oa PUBLIC include)

add_library(data_structures::hash_table_oa ALIAS data_structures_hash_table_oa)
target_compile_features(data_structures_hash_table_oa PUBLIC cxx_std_23)
