#include "bit_manipulation/bit_enum/bit_enum.h"

#include <algorithm>
#include <bit>
#include <gtest/gtest.h>
#include <set>

using namespace bit_manipulation::bit_enum;

// ---------- next_same_popcount (Gosper's hack) ----------

TEST(BitEnumTest, GosperBasic3of5) {
    // Enumerate all C(5,3)=10 three-element subsets of {0..4}
    uint32_t x = 0b00111; // first 3-bit subset
    const uint32_t limit = 1u << 5;
    std::vector<uint32_t> subsets;
    while (x < limit) {
        subsets.push_back(x);
        x = next_same_popcount(x);
    }
    EXPECT_EQ(subsets.size(), 10u);
    // Verify all have popcount 3
    for (auto s : subsets) {
        EXPECT_EQ(std::popcount(s), 3);
    }
    // Verify all are distinct
    std::set<uint32_t> unique(subsets.begin(), subsets.end());
    EXPECT_EQ(unique.size(), 10u);
}

TEST(BitEnumTest, GosperBasic1of4) {
    // C(4,1) = 4 one-element subsets
    uint32_t x = 0b0001;
    const uint32_t limit = 1u << 4;
    int count = 0;
    while (x < limit) {
        EXPECT_EQ(std::popcount(x), 1);
        x = next_same_popcount(x);
        ++count;
    }
    EXPECT_EQ(count, 4);
}

TEST(BitEnumTest, GosperPreservesPopcount) {
    uint32_t x = 0b1010;
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(std::popcount(x), 2);
        x = next_same_popcount(x);
    }
}

// ---------- enumerate_submasks ----------

TEST(BitEnumTest, SubmasksFull) {
    auto subs = collect_submasks(uint32_t{0b1010});
    // 2^2 = 4 submasks: 0b1010, 0b1000, 0b0010, 0b0000
    EXPECT_EQ(subs.size(), 4u);
    // All should be submasks of 0b1010
    for (auto s : subs) {
        EXPECT_EQ(s & ~uint32_t{0b1010}, 0u);
    }
    // Should contain 0
    EXPECT_NE(std::find(subs.begin(), subs.end(), 0u), subs.end());
    // Should contain mask itself
    EXPECT_NE(std::find(subs.begin(), subs.end(), uint32_t{0b1010}), subs.end());
}

TEST(BitEnumTest, SubmasksZero) {
    auto subs = collect_submasks(uint32_t{0});
    EXPECT_EQ(subs.size(), 1u);
    EXPECT_EQ(subs[0], 0u);
}

TEST(BitEnumTest, SubmasksSingleBit) {
    auto subs = collect_submasks(uint32_t{8});
    EXPECT_EQ(subs.size(), 2u); // 8, 0
}

TEST(BitEnumTest, SubmasksAllBits3) {
    auto subs = collect_submasks(uint32_t{0b111});
    EXPECT_EQ(subs.size(), 8u); // 2^3 = 8
    std::set<uint32_t> unique(subs.begin(), subs.end());
    EXPECT_EQ(unique.size(), 8u);
}

// ---------- Gray code ----------

TEST(BitEnumTest, GrayEncodeKnownValues) {
    EXPECT_EQ(gray_encode(0u), 0u);
    EXPECT_EQ(gray_encode(1u), 1u);
    EXPECT_EQ(gray_encode(2u), 3u);
    EXPECT_EQ(gray_encode(3u), 2u);
    EXPECT_EQ(gray_encode(4u), 6u);
    EXPECT_EQ(gray_encode(5u), 7u);
    EXPECT_EQ(gray_encode(6u), 5u);
    EXPECT_EQ(gray_encode(7u), 4u);
}

TEST(BitEnumTest, GrayDecodeIsInverse) {
    for (uint32_t i = 0; i < 1024; ++i) {
        EXPECT_EQ(gray_decode(gray_encode(i)), i);
    }
}

TEST(BitEnumTest, GrayAdjacentDifferByOneBit) {
    // Adjacent Gray codes must differ in exactly 1 bit
    for (uint32_t i = 0; i < 1023; ++i) {
        uint32_t diff = gray_encode(i) ^ gray_encode(i + 1);
        EXPECT_EQ(std::popcount(diff), 1);
    }
}

TEST(BitEnumTest, GrayCodeSequence3Bits) {
    auto seq = gray_code_sequence(3);
    EXPECT_EQ(seq.size(), 8u);
    // Expected: 0, 1, 3, 2, 6, 7, 5, 4
    std::vector<uint64_t> expected = {0, 1, 3, 2, 6, 7, 5, 4};
    EXPECT_EQ(seq, expected);
}

TEST(BitEnumTest, GrayCodeSequence0Bits) {
    auto seq = gray_code_sequence(0);
    EXPECT_EQ(seq.size(), 1u);
    EXPECT_EQ(seq[0], 0u);
}

TEST(BitEnumTest, GrayCodeSequenceAllUnique) {
    auto seq = gray_code_sequence(4);
    EXPECT_EQ(seq.size(), 16u);
    std::set<uint64_t> unique(seq.begin(), seq.end());
    EXPECT_EQ(unique.size(), 16u);
}

// ---------- constexpr checks ----------

TEST(BitEnumTest, ConstexprUsable) {
    static_assert(gray_encode(0u) == 0u);
    static_assert(gray_encode(3u) == 2u);
    static_assert(gray_decode(2u) == 3u);
    static_assert(next_same_popcount(uint32_t{0b0011}) == uint32_t{0b0101});
}

