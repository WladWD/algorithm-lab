file(GLOB HASH_MAP_TEST_SRC test_*.cpp)
add_executable(test_data_structures_lock_free_hash_map ${HASH_MAP_TEST_SRC})
target_link_libraries(test_data_structures_lock_free_hash_map PRIVATE
        data_structures::lock_free::hash_map
        GTest::gtest_main
)
add_test(NAME data_structures.lock_free.hash_map COMMAND test_data_structures_lock_free_hash_map)
