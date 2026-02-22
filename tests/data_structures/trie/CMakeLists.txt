add_executable(test_data_structures_trie test_trie.cpp)
target_link_libraries(test_data_structures_trie PRIVATE data_structures::trie GTest::gtest_main)
add_test(NAME data_structures.trie COMMAND test_data_structures_trie)

