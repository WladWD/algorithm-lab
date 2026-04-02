#include "bit_manipulation/bit_apps/bit_apps.h"

#include <gtest/gtest.h>
#include <vector>

using namespace bit_manipulation::bit_apps;

// ---------- xor_swap ----------

TEST(BitAppsTest, XorSwapBasic) {
    uint32_t a = 10, b = 20;
    xor_swap(a, b);
    EXPECT_EQ(a, 20u);
    EXPECT_EQ(b, 10u);
}

TEST(BitAppsTest, XorSwapZero) {
    uint64_t a = 0, b = 42;
    xor_swap(a, b);
    EXPECT_EQ(a, 42u);
    EXPECT_EQ(b, 0u);
}

TEST(BitAppsTest, XorSwapSameValue) {
    uint32_t a = 7, b = 7;
    xor_swap(a, b);
    EXPECT_EQ(a, 7u);
    EXPECT_EQ(b, 7u);
}

TEST(BitAppsTest, XorSwapSelfAlias) {
    uint32_t a = 42;
    xor_swap(a, a);  // self-swap guard should protect
    EXPECT_EQ(a, 42u);
}

TEST(BitAppsTest, XorSwapLargeValues) {
    uint64_t a = 0xDEADBEEFCAFEBABE;
    uint64_t b = 0x0123456789ABCDEF;
    xor_swap(a, b);
    EXPECT_EQ(a, 0x0123456789ABCDEFu);
    EXPECT_EQ(b, 0xDEADBEEFCAFEBABEu);
}

// ---------- xor_find_unique ----------

TEST(BitAppsTest, FindUniqueBasic) {
    std::vector<uint64_t> data = {2, 3, 5, 3, 2};
    EXPECT_EQ(xor_find_unique(data), 5u);
}

TEST(BitAppsTest, FindUniqueSingleElement) {
    std::vector<uint64_t> data = {42};
    EXPECT_EQ(xor_find_unique(data), 42u);
}

TEST(BitAppsTest, FindUniqueEmpty) {
    std::vector<uint64_t> data;
    EXPECT_EQ(xor_find_unique(data), 0u);
}

TEST(BitAppsTest, FindUniqueLargerSet) {
    std::vector<uint64_t> data = {1, 2, 3, 4, 5, 4, 3, 2, 1};
    EXPECT_EQ(xor_find_unique(data), 5u);
}

TEST(BitAppsTest, FindUniqueAllPaired) {
    std::vector<uint64_t> data = {10, 20, 10, 20};
    EXPECT_EQ(xor_find_unique(data), 0u);
}

// ---------- bitwise_sieve ----------

TEST(BitAppsTest, SieveSmall) {
    auto sieve = bitwise_sieve(20);
    // Primes up to 20: 2, 3, 5, 7, 11, 13, 17, 19
    std::vector<uint64_t> expected_primes = {2, 3, 5, 7, 11, 13, 17, 19};
    for (auto p : expected_primes) {
        EXPECT_TRUE(is_prime_in_sieve(sieve, p)) << p << " should be prime";
    }
    // Non-primes
    std::vector<uint64_t> composites = {0, 1, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20};
    for (auto c : composites) {
        EXPECT_FALSE(is_prime_in_sieve(sieve, c)) << c << " should not be prime";
    }
}

TEST(BitAppsTest, SieveTwo) {
    auto sieve = bitwise_sieve(2);
    EXPECT_FALSE(is_prime_in_sieve(sieve, 0));
    EXPECT_FALSE(is_prime_in_sieve(sieve, 1));
    EXPECT_TRUE(is_prime_in_sieve(sieve, 2));
}

TEST(BitAppsTest, SieveUpTo100) {
    auto sieve = bitwise_sieve(100);
    // Known: 25 primes up to 100
    uint64_t count = 0;
    for (uint64_t i = 0; i <= 100; ++i) {
        if (is_prime_in_sieve(sieve, i)) ++count;
    }
    EXPECT_EQ(count, 25u);
}

TEST(BitAppsTest, SieveLarger) {
    // 1000: 168 primes
    auto sieve = bitwise_sieve(1000);
    uint64_t count = 0;
    for (uint64_t i = 0; i <= 1000; ++i) {
        if (is_prime_in_sieve(sieve, i)) ++count;
    }
    EXPECT_EQ(count, 168u);
}

// ---------- count_primes ----------

TEST(BitAppsTest, CountPrimesSmall) {
    EXPECT_EQ(count_primes(0), 0u);
    EXPECT_EQ(count_primes(1), 0u);
    EXPECT_EQ(count_primes(2), 1u);
    EXPECT_EQ(count_primes(10), 4u);
    EXPECT_EQ(count_primes(20), 8u);
}

TEST(BitAppsTest, CountPrimes100) {
    EXPECT_EQ(count_primes(100), 25u);
}

TEST(BitAppsTest, CountPrimes1000) {
    EXPECT_EQ(count_primes(1000), 168u);
}

TEST(BitAppsTest, CountPrimes10000) {
    EXPECT_EQ(count_primes(10000), 1229u);
}

