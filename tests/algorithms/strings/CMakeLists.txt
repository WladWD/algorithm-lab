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

add_executable(test_strings_kmp kmp/test_kmp.cpp)
target_link_libraries(test_strings_kmp PRIVATE
        algo_strings_kmp
        GTest::gtest_main
)
add_test(NAME strings.kmp COMMAND test_strings_kmp)

add_executable(test_strings_z_function z_function/test_z_function.cpp)
target_link_libraries(test_strings_z_function PRIVATE
        algo_strings_z_function
        GTest::gtest_main
)
add_test(NAME strings.z_function COMMAND test_strings_z_function)

add_executable(test_strings_aho_corasick aho_corasick/test_aho_corasick.cpp)
target_link_libraries(test_strings_aho_corasick PRIVATE
        algo_strings_aho_corasick
        GTest::gtest_main
)
add_test(NAME strings.aho_corasick COMMAND test_strings_aho_corasick)

add_executable(test_strings_rabin_karp rabin_karp/test_rabin_karp.cpp)
target_link_libraries(test_strings_rabin_karp PRIVATE
        algo_strings_rabin_karp
        GTest::gtest_main
)
add_test(NAME strings.rabin_karp COMMAND test_strings_rabin_karp)



