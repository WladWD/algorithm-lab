#include <algorithms/strings/rabin_karp/rabin_karp.h>

#include <gtest/gtest.h>
#include <vector>

namespace rk = algorithms::strings::rabin_karp;

// ==================== search ====================

TEST(RabinKarp, SearchBasic) {
    auto pos = rk::search("abracadabra", "abra");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 7}));
}

TEST(RabinKarp, SearchOverlapping) {
    auto pos = rk::search("aaaa", "aa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

TEST(RabinKarp, SearchNoMatch) {
    auto pos = rk::search("abcdef", "xyz");
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, SearchEmptyPattern) {
    auto pos = rk::search("hello", "");
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, SearchEmptyText) {
    auto pos = rk::search("", "abc");
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, SearchPatternEqualsText) {
    auto pos = rk::search("hello", "hello");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0}));
}

TEST(RabinKarp, SearchPatternLongerThanText) {
    auto pos = rk::search("hi", "hello");
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, SearchSingleChar) {
    auto pos = rk::search("abacaba", "a");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4, 6}));
}

TEST(RabinKarp, SearchRepeated) {
    auto pos = rk::search("ababababab", "ab");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4, 6, 8}));
}

TEST(RabinKarp, SearchAllSame) {
    auto pos = rk::search("aaaaa", "aaa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

TEST(RabinKarp, SearchAtEnd) {
    auto pos = rk::search("xxxxxabc", "abc");
    EXPECT_EQ(pos, (std::vector<std::size_t>{5}));
}

TEST(RabinKarp, SearchAtStart) {
    auto pos = rk::search("abcxxxxx", "abc");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0}));
}

// ==================== count_occurrences ====================

TEST(RabinKarp, CountOverlapping) {
    EXPECT_EQ(rk::count_occurrences("aaaa", "aa"), 3u);
}

TEST(RabinKarp, CountNone) {
    EXPECT_EQ(rk::count_occurrences("abcdef", "xyz"), 0u);
}

TEST(RabinKarp, CountMultiple) {
    EXPECT_EQ(rk::count_occurrences("abracadabra", "abra"), 2u);
}

TEST(RabinKarp, CountSingle) {
    EXPECT_EQ(rk::count_occurrences("hello", "hello"), 1u);
}

// ==================== search_multiple ====================

TEST(RabinKarp, SearchMultipleBasic) {
    // "abc" at 0,3 and "bca" at 1 — all length 3
    auto pos = rk::search_multiple("abcabc", {"abc", "bca"});
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 3}));
}

TEST(RabinKarp, SearchMultipleSinglePattern) {
    auto pos = rk::search_multiple("abracadabra", {"abra"});
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 7}));
}

TEST(RabinKarp, SearchMultipleNoMatch) {
    auto pos = rk::search_multiple("abcdef", {"xyz", "www"});
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, SearchMultipleEmpty) {
    auto pos = rk::search_multiple("abcdef", {});
    EXPECT_TRUE(pos.empty());
}

TEST(RabinKarp, SearchMultipleDuplicatePatterns) {
    // Duplicate patterns — each position matched only once
    auto pos = rk::search_multiple("abab", {"ab", "ab"});
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2}));
}

TEST(RabinKarp, SearchMultipleDisjointPatterns) {
    // "aa" at 0,1,2 and "bb" at 3,4,5
    auto pos = rk::search_multiple("aaabbb", {"aa", "bb"});
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 3, 4}));
}

// ==================== Agreement with KMP ====================

TEST(RabinKarp, MatchesKMPResults) {
    std::string text = "abcabcabcabcabc";
    std::string pat  = "abcabc";
    auto pos = rk::search(text, pat);
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 3, 6, 9}));
}
