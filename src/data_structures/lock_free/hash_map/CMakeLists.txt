# Add a simple static library target for the lock-free hash_map
file(GLOB SRC src/*.cpp)
add_library(data_structures_lock_free_hash_map STATIC ${SRC})
target_include_directories(data_structures_lock_free_hash_map PUBLIC include)

add_library(data_structures::lock_free::hash_map ALIAS data_structures_lock_free_hash_map)
target_compile_features(data_structures_lock_free_hash_map PUBLIC cxx_std_23)

