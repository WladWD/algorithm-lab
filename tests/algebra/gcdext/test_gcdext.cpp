#include "algebra/gcdext/gcdext.h"

#include <gtest/gtest.h>
#include <tuple>

using namespace algebra::gcdext;

TEST(GcdExtTest, ExtendedCoefficients) {
    auto [g, x, y] = extended_gcd(30, 20);
    EXPECT_EQ(g, 10);
    EXPECT_EQ(30 * x + 20 * y, g);

    auto [g2, x2, y2] = extended_gcd(101, 462);
    EXPECT_EQ(g2, get<0>(extended_gcd(101, 462)));
    EXPECT_EQ(101 * x2 + 462 * y2, g2);
}

TEST(GcdExtTest, DiophantineSolution) {
    // Solve 6*x + 15*y = 3
    auto [g, x0, y0] = extended_gcd(6, 15);
    ASSERT_EQ(g, 3);
    int64_t c = 3;
    int64_t x = x0 * (c / g);
    int64_t y = y0 * (c / g);
    EXPECT_EQ(6 * x + 15 * y, c);
}

// No main: using GTest main provided by GTest::gtest_main
