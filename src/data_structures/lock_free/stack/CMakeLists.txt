# ---- Dijkstra module  ----
file(GLOB SRC src/*.cpp)
add_library(data_structures_lock_free_stack STATIC ${SRC})
target_include_directories(data_structures_lock_free_stack PUBLIC include)

add_library(data_structures::lock_free::stack ALIAS data_structures_lock_free_stack)
target_compile_features(data_structures_lock_free_stack PUBLIC cxx_std_23)
