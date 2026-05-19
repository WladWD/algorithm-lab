# ---- Ordered Map (AVL tree) module ----
file(GLOB SRC src/*.cpp)
add_library(data_structures_ordered_map STATIC ${SRC})
target_include_directories(data_structures_ordered_map PUBLIC include)

add_library(data_structures::ordered_map ALIAS data_structures_ordered_map)
target_compile_features(data_structures_ordered_map PUBLIC cxx_std_23)
