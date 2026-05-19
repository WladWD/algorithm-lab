add_executable(test_data_structures_hash_table_chaining test_hash_table_chaining.cpp)
target_link_libraries(test_data_structures_hash_table_chaining PRIVATE
        data_structures::hash_table_chaining
        GTest::gtest_main
)
add_test(NAME data_structures.hash_table_chaining COMMAND test_data_structures_hash_table_chaining)
