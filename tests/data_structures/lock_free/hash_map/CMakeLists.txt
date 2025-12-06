# Tests for lock-free hash_map
add_executable(test_lock_free_hash_map hash_map.cpp)
target_link_libraries(test_lock_free_hash_map PRIVATE data_structures::lock_free::hash_map GTest::gtest_main)

group_tests("data_structures/lock_free")

