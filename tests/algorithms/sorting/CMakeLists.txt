add_executable(test_sorting_quicksort quicksort/test_quicksort.cpp)
target_link_libraries(test_sorting_quicksort PRIVATE
        algo_sorting_quicksort
        GTest::gtest_main
)
add_test(NAME sorting.quicksort COMMAND test_sorting_quicksort)

