add_executable(test_garner garner/test_garner.cpp)
target_link_libraries(test_garner PRIVATE algo_algebra_garner GTest::gtest_main)
add_test(NAME graphs.dijkstra COMMAND test_garner)

add_executable(test_gcd gcd/test_gcd.cpp)
target_link_libraries(test_gcd PRIVATE algo_algebra_gcd GTest::gtest_main)
add_test(NAME algebra.gcd COMMAND test_gcd)

add_executable(test_gcdext gcdext/test_gcdext.cpp)
target_link_libraries(test_gcdext PRIVATE algo_algebra_gcdext GTest::gtest_main)
add_test(NAME algebra.gcdext COMMAND test_gcdext)

add_executable(test_mod_inv mod_inv/test_mod_inv.cpp)
target_link_libraries(test_mod_inv PRIVATE algo_algebra_mod_inv GTest::gtest_main)
add_test(NAME algebra.mod_inv COMMAND test_mod_inv)