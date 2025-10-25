add_executable(test_data_structures_lock_free_stack stack.cpp)
target_link_libraries(test_data_structures_lock_free_stack PRIVATE
        data_structures::lock_free::stack
        GTest::gtest_main
)
add_test(NAME data_structures.lock_free.stack COMMAND test_data_structures_lock_free_stack)