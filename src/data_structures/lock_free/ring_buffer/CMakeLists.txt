file(GLOB SRC src/*.cpp)
add_library(data_structures_lock_free_ring_buffer STATIC ${SRC})
target_include_directories(data_structures_lock_free_ring_buffer PUBLIC include)

add_library(data_structures::lock_free::ring_buffer ALIAS data_structures_lock_free_ring_buffer)
target_compile_features(data_structures_lock_free_ring_buffer PUBLIC cxx_std_23)
