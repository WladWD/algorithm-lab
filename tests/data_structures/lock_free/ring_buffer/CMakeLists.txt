file(GLOB RING_BUFFER_TEST_SRC test_*.cpp)
add_executable(test_data_structures_lock_free_ring_buffer ${RING_BUFFER_TEST_SRC})
target_link_libraries(test_data_structures_lock_free_ring_buffer PRIVATE
        data_structures::lock_free::ring_buffer
        GTest::gtest_main
)
add_test(NAME data_structures.lock_free.ring_buffer COMMAND test_data_structures_lock_free_ring_buffer)
