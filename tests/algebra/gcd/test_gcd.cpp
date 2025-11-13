#include <gtest/gtest.h>
#include "algebra/gcd/gcd.h"
#include <tuple>

using namespace algebra::gcd;

TEST(GcdExtTest, GcdBasics) {
    EXPECT_EQ(gcd(48, 18), 6);
    EXPECT_EQ(gcd(18, 48), 6);
    EXPECT_EQ(gcd(0, 5), 5);
    EXPECT_EQ(gcd(0, 0), 0);
    EXPECT_EQ(gcd(-24, 18), 6);
}

// No main: using GTest main provided by GTest::gtest_main

