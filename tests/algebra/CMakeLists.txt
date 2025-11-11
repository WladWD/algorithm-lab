add_executable(test_garner garner/test_garner.cpp)
target_link_libraries(test_garner PRIVATE
        algo_algebra_garner
        GTest::gtest_main
)
add_test(NAME graphs.dijkstra COMMAND test_garner)