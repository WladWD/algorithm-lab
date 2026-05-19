add_executable(test_data_structures_ordered_map test_ordered_map.cpp)
target_link_libraries(test_data_structures_ordered_map PRIVATE
        data_structures::ordered_map
        GTest::gtest_main
)
add_test(NAME data_structures.ordered_map COMMAND test_data_structures_ordered_map)
