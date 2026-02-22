file(GLOB SRC src/*.cpp)
add_library(data_structures_trie STATIC ${SRC})
target_include_directories(data_structures_trie PUBLIC include)

target_compile_features(data_structures_trie PUBLIC cxx_std_23)
add_library(data_structures::trie ALIAS data_structures_trie)

