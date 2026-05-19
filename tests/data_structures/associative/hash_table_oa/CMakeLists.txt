add_executable(test_data_structures_hash_table_oa test_hash_table_oa.cpp)
target_link_libraries(test_data_structures_hash_table_oa PRIVATE
        data_structures::hash_table_oa
        GTest::gtest_main
)
add_test(NAME data_structures.hash_table_oa COMMAND test_data_structures_hash_table_oa)
