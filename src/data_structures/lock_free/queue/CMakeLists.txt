file(GLOB SRC src/*.cpp)
add_library(data_structures_lock_free_queue STATIC ${SRC})
target_include_directories(data_structures_lock_free_queue PUBLIC include)

add_library(data_structures::lock_free::queue ALIAS data_structures_lock_free_queue)
target_compile_features(data_structures_lock_free_queue PUBLIC cxx_std_23)
