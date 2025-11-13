add_executable(test_gcdext test_gcdext.cpp)

target_link_libraries(test_gcdext
    PRIVATE
        GTest::gtest_main
)

target_include_directories(test_gcdext PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME algebra.gcdext COMMAND test_gcdext)

