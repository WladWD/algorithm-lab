# ---- Hash Table (chaining) module ----
file(GLOB SRC src/*.cpp)
add_library(data_structures_hash_table_chaining STATIC ${SRC})
target_include_directories(data_structures_hash_table_chaining PUBLIC include)

add_library(data_structures::hash_table_chaining ALIAS data_structures_hash_table_chaining)
target_compile_features(data_structures_hash_table_chaining PUBLIC cxx_std_23)
