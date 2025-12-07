file(GLOB_RECURSE STACK_SRC stack/*.cpp)
add_executable(test_data_structures_lock_free_stack ${STACK_SRC})
target_link_libraries(test_data_structures_lock_free_stack PRIVATE
        data_structures::lock_free::stack
        GTest::gtest_main
)
add_test(NAME data_structures.lock_free.stack COMMAND test_data_structures_lock_free_stack)

file(GLOB_RECURSE QUEUE_SRC queue/*.cpp)
add_executable(test_data_structures_lock_free_queue ${QUEUE_SRC})
target_link_libraries(test_data_structures_lock_free_queue PRIVATE
        data_structures::lock_free::queue
        GTest::gtest_main
)
add_test(NAME data_structures.lock_free.queue COMMAND test_data_structures_lock_free_queue)

add_subdirectory(ring_buffer)
add_subdirectory(hazard_pointers)
add_subdirectory(hash_map)
