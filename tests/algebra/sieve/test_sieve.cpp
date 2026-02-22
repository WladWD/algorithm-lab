#include "algebra/sieve/sieve.h"

#include <gtest/gtest.h>

using namespace algebra::sieve;

TEST(Sieve, SmallN_PrimesList) {
    EXPECT_TRUE(primes_up_to(0).empty());
    EXPECT_TRUE(primes_up_to(1).empty());
    EXPECT_EQ(primes_up_to(2), (std::vector<uint32_t>{2}));
    EXPECT_EQ(primes_up_to(3), (std::vector<uint32_t>{2, 3}));
}

TEST(Sieve, KnownPrimesUpTo30) {
    EXPECT_EQ(primes_up_to(30), (std::vector<uint32_t>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29}));
}

TEST(Sieve, IsPrimeTableBasics) {
    auto t = is_prime_up_to(10);
    ASSERT_EQ(t.size(), 11u);

    EXPECT_EQ(t[0], 0);
    EXPECT_EQ(t[1], 0);
    EXPECT_EQ(t[2], 1);
    EXPECT_EQ(t[3], 1);
    EXPECT_EQ(t[4], 0);
    EXPECT_EQ(t[5], 1);
    EXPECT_EQ(t[6], 0);
    EXPECT_EQ(t[7], 1);
    EXPECT_EQ(t[8], 0);
    EXPECT_EQ(t[9], 0);
    EXPECT_EQ(t[10], 0);
}

TEST(Sieve, ConsistencyListVsTable) {
    const std::size_t n = 500;
    auto t = is_prime_up_to(n);
    auto primes = primes_up_to(n);

    std::size_t count = 0;
    for (std::size_t i = 2; i <= n; ++i) {
        if (t[i])
            ++count;
    }
    EXPECT_EQ(count, primes.size());

    for (auto p : primes) {
        ASSERT_LE(p, n);
        EXPECT_EQ(t[p], 1);
    }
}
