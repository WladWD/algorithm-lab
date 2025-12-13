file(GLOB SRC src/*.cpp)
add_library(data_structures_lock_free_barrier STATIC ${SRC})
target_include_directories(data_structures_lock_free_barrier PUBLIC include)

add_library(data_structures::lock_free::barrier ALIAS data_structures_lock_free_barrier)
target_compile_features(data_structures_lock_free_barrier PUBLIC cxx_std_23)

