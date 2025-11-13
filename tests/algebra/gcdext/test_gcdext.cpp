#include <gtest/gtest.h>
#include "algebra/gcdext/gcdext.h"
#include <tuple>

using namespace algebra::gcdext;

TEST(GcdExtTest, GcdBasics) {
    EXPECT_EQ(gcd(48, 18), 6);
    EXPECT_EQ(gcd(18, 48), 6);
    EXPECT_EQ(gcd(0, 5), 5);
    EXPECT_EQ(gcd(0, 0), 0);
    EXPECT_EQ(gcd(-24, 18), 6);
}

TEST(GcdExtTest, ExtendedCoefficients) {
    auto [g, x, y] = extended_gcd(30, 20);
    EXPECT_EQ(g, 10);
    EXPECT_EQ(30 * x + 20 * y, g);

    // random check
    auto [g2, x2, y2] = extended_gcd(101, 462);
    EXPECT_EQ(g2, gcd(101, 462));
    EXPECT_EQ(101 * x2 + 462 * y2, g2);
}

TEST(GcdExtTest, ModInv) {
    EXPECT_EQ(modinv(3, 11), 4u);
    EXPECT_EQ(modinv(10, 17), 12u); // 10*12=120 ≡ 1 (mod 17)
    // non-invertible case
    EXPECT_THROW(modinv(6, 9), std::invalid_argument);
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

