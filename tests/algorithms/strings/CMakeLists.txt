add_executable(test_strings_polynomial_hash polynomial_hash/test_polynomial_hash.cpp)
target_link_libraries(test_strings_polynomial_hash PRIVATE
        algo_strings_polynomial_hash
        GTest::gtest_main
)
add_test(NAME strings.polynomial_hash COMMAND test_strings_polynomial_hash)

add_executable(test_strings_rolling_hash rolling_hash/test_rolling_hash.cpp)
target_link_libraries(test_strings_rolling_hash PRIVATE
        algo_strings_rolling_hash
        GTest::gtest_main
)
add_test(NAME strings.rolling_hash COMMAND test_strings_rolling_hash)

