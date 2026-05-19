#include <algorithms/strings/rolling_hash/rolling_hash.h>

#include <algorithm>
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace rh = algorithms::strings::rolling_hash;

// ==================== RollingHash basic ====================

TEST(RollingHash, InitialValue) {
    rh::RollingHash h("abcde", 3);
    // Hash of "abc" should be deterministic
    rh::RollingHash ref("abc", 3);
    EXPECT_EQ(h.value(), ref.value());
}

TEST(RollingHash, SlideProducesCorrectHash) {
    rh::RollingHash h("abcde", 3);
    h.slide('a', 'd');  // now hashing "bcd"

    rh::RollingHash ref("bcd", 3);
    EXPECT_EQ(h.value(), ref.value());
}

TEST(RollingHash, SlideMultipleTimes) {
    std::string s = "abcdefgh";
    rh::RollingHash h(s, 4);  // starts at "abcd"

    for (std::size_t i = 1; i + 4 <= s.size(); ++i) {
        h.slide(s[i - 1], s[i + 3]);
        rh::RollingHash ref(s.substr(i, 4), 4);
        EXPECT_EQ(h.value(), ref.value()) << "i=" << i;
    }
}

TEST(RollingHash, WindowSizeOne) {
    rh::RollingHash h("abc", 1);
    rh::RollingHash ref_a("a", 1), ref_b("b", 1), ref_c("c", 1);
    EXPECT_EQ(h.value(), ref_a.value());
    h.slide('a', 'b');
    EXPECT_EQ(h.value(), ref_b.value());
    h.slide('b', 'c');
    EXPECT_EQ(h.value(), ref_c.value());
}

TEST(RollingHash, WindowEqualsString) {
    rh::RollingHash h("hello", 5);
    rh::RollingHash ref("hello", 5);
    EXPECT_EQ(h.value(), ref.value());
}

// ==================== DoubleRollingHash ====================

TEST(DoubleRollingHash, SlideCorrect) {
    std::string s = "abcdefgh";
    rh::DoubleRollingHash h(s, 3);

    for (std::size_t i = 1; i + 3 <= s.size(); ++i) {
        h.slide(s[i - 1], s[i + 2]);
        rh::DoubleRollingHash ref(s.substr(i, 3), 3);
        EXPECT_EQ(h.value(), ref.value()) << "i=" << i;
    }
}

TEST(DoubleRollingHash, DifferentWindowsHaveDifferentHashes) {
    rh::DoubleRollingHash h("abcde", 3);
    auto h1 = h.value();
    h.slide('a', 'd');
    auto h2 = h.value();
    EXPECT_NE(h1, h2);
}

// ==================== rabin_karp ====================

TEST(RabinKarp, BasicPattern) {
    auto pos = rh::rabin_karp("abracadabra", "abra");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 7}));
}

TEST(RabinKarp, OverlappingMatches) {
    auto pos = rh::rabin_karp("aaaa", "aa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

TEST(RabinKarp, NoMatch) {
    auto pos = rh::rabin_karp("abcdefg", "xyz");
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, PatternEqualsText) {
    auto pos = rh::rabin_karp("hello", "hello");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0}));
}

TEST(RabinKarp, PatternLongerThanText) {
    auto pos = rh::rabin_karp("hi", "hello");
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, EmptyPattern) {
    auto pos = rh::rabin_karp("hello", "");
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, SingleChar) {
    auto pos = rh::rabin_karp("abacaba", "a");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4, 6}));
}

TEST(RabinKarp, PatternAtEnd) {
    auto pos = rh::rabin_karp("xxxxabc", "abc");
    EXPECT_EQ(pos, (std::vector<std::size_t>{4}));
}

TEST(RabinKarp, RepeatedPattern) {
    auto pos = rh::rabin_karp("ababababab", "ab");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4, 6, 8}));
}

TEST(RabinKarp, AllSameCharacters) {
    auto pos = rh::rabin_karp("aaaaa", "aaa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

TEST(RabinKarp, LargeText) {
    // Build text "abcabc...abc" (repeated 100 times) and find "abc"
    std::string t;
    for (int i = 0; i < 100; ++i) t += "abc";
    auto pos = rh::rabin_karp(t, "abc");
    ASSERT_EQ(pos.size(), 100u);
    for (std::size_t i = 0; i < 100; ++i) {
        EXPECT_EQ(pos[i], i * 3);
    }
}

// ==================== longest_duplicate_substring ====================

TEST(LongestDuplicate, Banana) {
    // "banana" — longest duplicate is "ana" (length 3)
    std::size_t len = rh::longest_duplicate_substring("banana");
    EXPECT_EQ(len, 3u);
}

TEST(LongestDuplicate, Abcabc) {
    // "abcabc" — "abc" appears twice, length 3
    std::size_t len = rh::longest_duplicate_substring("abcabc");
    EXPECT_EQ(len, 3u);
}

TEST(LongestDuplicate, AllSame) {
    // "aaaa" — longest duplicate is "aaa" (length 3)
    std::size_t len = rh::longest_duplicate_substring("aaaa");
    EXPECT_EQ(len, 3u);
}

TEST(LongestDuplicate, NoDuplicate) {
    // "abcde" — no duplicate substring
    std::size_t len = rh::longest_duplicate_substring("abcde");
    EXPECT_EQ(len, 0u);
}

TEST(LongestDuplicate, SingleChar) {
    std::size_t len = rh::longest_duplicate_substring("a");
    EXPECT_EQ(len, 0u);
}

TEST(LongestDuplicate, TwoSameChars) {
    // "aa" — "a" appears twice, length 1
    std::size_t len = rh::longest_duplicate_substring("aa");
    EXPECT_EQ(len, 1u);
}

