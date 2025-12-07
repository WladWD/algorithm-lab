file(GLOB_RECURSE QUEUE_SRC *.cpp)
add_executable(test_data_structures_lock_free_queue ${QUEUE_SRC})
target_link_libraries(test_data_structures_lock_free_queue PRIVATE
        data_structures::lock_free::queue
        GTest::gtest_main
)
add_test(NAME data_structures.lock_free.queue COMMAND test_data_structures_lock_free_queue)