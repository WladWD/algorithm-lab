file(GLOB_RECURSE TEST_SRC *.cpp)

add_executable(test_data_structures_lock_free ${TEST_SRC})
target_link_libraries(test_data_structures_lock_free PRIVATE
        data_structures::lock_free::stack
        GTest::gtest_main
)
add_test(NAME data_structures_lock_free COMMAND test_data_structures_lock_free)