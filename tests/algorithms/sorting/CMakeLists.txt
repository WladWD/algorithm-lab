add_executable(test_sorting_quicksort quicksort/test_quicksort.cpp)
target_link_libraries(test_sorting_quicksort PRIVATE
        algo_sorting_quicksort
        GTest::gtest_main
)
add_test(NAME sorting.quicksort COMMAND test_sorting_quicksort)

add_executable(test_sorting_mergesort mergesort/test_mergesort.cpp)
target_link_libraries(test_sorting_mergesort PRIVATE
        algo_sorting_mergesort
        GTest::gtest_main
)
add_test(NAME sorting.mergesort COMMAND test_sorting_mergesort)

add_executable(test_sorting_heapsort heapsort/test_heapsort.cpp)
target_link_libraries(test_sorting_heapsort PRIVATE
        algo_sorting_heapsort
        GTest::gtest_main
)
add_test(NAME sorting.heapsort COMMAND test_sorting_heapsort)

add_executable(test_sorting_counting_sort counting_sort/test_counting_sort.cpp)
target_link_libraries(test_sorting_counting_sort PRIVATE
        algo_sorting_counting_sort
        GTest::gtest_main
)
add_test(NAME sorting.counting_sort COMMAND test_sorting_counting_sort)

add_executable(test_sorting_radix_sort radix_sort/test_radix_sort.cpp)
target_link_libraries(test_sorting_radix_sort PRIVATE
        algo_sorting_radix_sort
        GTest::gtest_main
)
add_test(NAME sorting.radix_sort COMMAND test_sorting_radix_sort)

add_executable(test_sorting_binary_search binary_search/test_binary_search.cpp)
target_link_libraries(test_sorting_binary_search PRIVATE
        algo_sorting_binary_search
        GTest::gtest_main
)
add_test(NAME sorting.binary_search COMMAND test_sorting_binary_search)

add_executable(test_sorting_interpolation_search interpolation_search/test_interpolation_search.cpp)
target_link_libraries(test_sorting_interpolation_search PRIVATE
        algo_sorting_interpolation_search
        GTest::gtest_main
)
add_test(NAME sorting.interpolation_search COMMAND test_sorting_interpolation_search)

add_executable(test_sorting_ternary_search ternary_search/test_ternary_search.cpp)
target_link_libraries(test_sorting_ternary_search PRIVATE
        algo_sorting_ternary_search
        GTest::gtest_main
)
add_test(NAME sorting.ternary_search COMMAND test_sorting_ternary_search)

add_executable(test_sorting_quickselect quickselect/test_quickselect.cpp)
target_link_libraries(test_sorting_quickselect PRIVATE
        algo_sorting_quickselect
        GTest::gtest_main
)
add_test(NAME sorting.quickselect COMMAND test_sorting_quickselect)

