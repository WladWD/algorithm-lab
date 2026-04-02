#include "bit_manipulation/bit_tricks/bit_tricks.h"

#include <gtest/gtest.h>

using namespace bit_manipulation::bit_tricks;

// ---------- popcount ----------

TEST(BitTricksTest, PopcountZero) {
    EXPECT_EQ(popcount(0u), 0);
    EXPECT_EQ(popcount(uint64_t{0}), 0);
}

TEST(BitTricksTest, PopcountSingleBit) {
    EXPECT_EQ(popcount(1u), 1);
    EXPECT_EQ(popcount(uint64_t{1} << 63), 1);
}

TEST(BitTricksTest, PopcountMultipleBits) {
    EXPECT_EQ(popcount(uint32_t{0b10110}), 3);
    EXPECT_EQ(popcount(uint8_t{0xFF}), 8);
    EXPECT_EQ(popcount(uint64_t{0xFFFFFFFFFFFFFFFF}), 64);
}

TEST(BitTricksTest, PopcountArbitrary) {
    EXPECT_EQ(popcount(uint32_t{12345}), 6);  // 11000000111001
    EXPECT_EQ(popcount(uint32_t{7}), 3);       // 111
}

// ---------- is_power_of_two ----------

TEST(BitTricksTest, IsPowerOfTwoTrue) {
    EXPECT_TRUE(is_power_of_two(1u));
    EXPECT_TRUE(is_power_of_two(2u));
    EXPECT_TRUE(is_power_of_two(4u));
    EXPECT_TRUE(is_power_of_two(uint64_t{1} << 32));
    EXPECT_TRUE(is_power_of_two(uint64_t{1} << 63));
}

TEST(BitTricksTest, IsPowerOfTwoFalse) {
    EXPECT_FALSE(is_power_of_two(0u));
    EXPECT_FALSE(is_power_of_two(3u));
    EXPECT_FALSE(is_power_of_two(5u));
    EXPECT_FALSE(is_power_of_two(6u));
    EXPECT_FALSE(is_power_of_two(15u));
    EXPECT_FALSE(is_power_of_two(uint64_t{0xFFFFFFFFFFFFFFFF}));
}

// ---------- lowest_set_bit ----------

TEST(BitTricksTest, LowestSetBitZero) {
    EXPECT_EQ(lowest_set_bit(0u), 0u);
}

TEST(BitTricksTest, LowestSetBitBasic) {
    EXPECT_EQ(lowest_set_bit(uint32_t{0b10100}), uint32_t{0b00100});
    EXPECT_EQ(lowest_set_bit(uint32_t{0b10000}), uint32_t{0b10000});
    EXPECT_EQ(lowest_set_bit(uint32_t{1}), uint32_t{1});
    EXPECT_EQ(lowest_set_bit(uint32_t{12}), uint32_t{4}); // 1100 -> 0100
}

TEST(BitTricksTest, LowestSetBit64) {
    EXPECT_EQ(lowest_set_bit(uint64_t{0x8000000000000000}),
              uint64_t{0x8000000000000000});
    EXPECT_EQ(lowest_set_bit(uint64_t{6}), uint64_t{2}); // 110 -> 010
}

// ---------- highest_set_bit ----------

TEST(BitTricksTest, HighestSetBitZero) {
    EXPECT_EQ(highest_set_bit(0u), 0u);
}

TEST(BitTricksTest, HighestSetBitBasic) {
    EXPECT_EQ(highest_set_bit(uint32_t{0b10110}), uint32_t{0b10000});
    EXPECT_EQ(highest_set_bit(uint32_t{1}), uint32_t{1});
    EXPECT_EQ(highest_set_bit(uint32_t{255}), uint32_t{128});
    EXPECT_EQ(highest_set_bit(uint32_t{256}), uint32_t{256});
}

TEST(BitTricksTest, HighestSetBit64) {
    EXPECT_EQ(highest_set_bit(uint64_t{0x8000000000000000}),
              uint64_t{0x8000000000000000});
    EXPECT_EQ(highest_set_bit(uint64_t{0xFFFFFFFFFFFFFFFF}),
              uint64_t{0x8000000000000000});
}

// ---------- clear_lowest_set_bit ----------

TEST(BitTricksTest, ClearLowestSetBitZero) {
    EXPECT_EQ(clear_lowest_set_bit(0u), 0u);
}

TEST(BitTricksTest, ClearLowestSetBitBasic) {
    EXPECT_EQ(clear_lowest_set_bit(uint32_t{0b10100}), uint32_t{0b10000});
    EXPECT_EQ(clear_lowest_set_bit(uint32_t{0b10000}), uint32_t{0b00000});
    EXPECT_EQ(clear_lowest_set_bit(uint32_t{1}), uint32_t{0});
    EXPECT_EQ(clear_lowest_set_bit(uint32_t{7}), uint32_t{6}); // 111 -> 110
}

TEST(BitTricksTest, ClearLowestSetBitCountsBits) {
    // Repeatedly clearing lowest bit counts set bits (Brian Kernighan)
    uint32_t x = 0b110101;
    int count = 0;
    while (x != 0) {
        x = clear_lowest_set_bit(x);
        ++count;
    }
    EXPECT_EQ(count, 4);
}

// ---------- constexpr checks ----------

TEST(BitTricksTest, ConstexprUsable) {
    static_assert(popcount(uint32_t{7}) == 3);
    static_assert(is_power_of_two(uint32_t{8}));
    static_assert(!is_power_of_two(uint32_t{0}));
    static_assert(lowest_set_bit(uint32_t{12}) == 4);
    static_assert(highest_set_bit(uint32_t{12}) == 8);
    static_assert(clear_lowest_set_bit(uint32_t{12}) == 8);
}

