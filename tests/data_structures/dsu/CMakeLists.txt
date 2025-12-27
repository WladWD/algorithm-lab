add_executable(test_data_structures_dsu test_dsu.cpp)
target_link_libraries(test_data_structures_dsu PRIVATE data_structures::dsu GTest::gtest_main)
add_test(NAME data_structures.dsu COMMAND test_data_structures_dsu)

