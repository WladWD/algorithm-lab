file(GLOB SRC src/*.cpp)
add_library(data_structures_lock_free_spinlock STATIC ${SRC})
target_include_directories(data_structures_lock_free_spinlock PUBLIC include)

add_library(data_structures::lock_free::spinlock ALIAS data_structures_lock_free_spinlock)
target_compile_features(data_structures_lock_free_spinlock PUBLIC cxx_std_23)

