#include <algorithms/strings/kmp/kmp.h>

#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

namespace kmp = algorithms::strings::kmp;

// ==================== prefix_function ====================

TEST(KMP, PrefixFunctionEmpty) {
    auto pi = kmp::prefix_function("");
    EXPECT_TRUE(pi.empty());
}

TEST(KMP, PrefixFunctionSingle) {
    auto pi = kmp::prefix_function("a");
    EXPECT_EQ(pi, (std::vector<int>{0}));
}

TEST(KMP, PrefixFunctionAllSame) {
    auto pi = kmp::prefix_function("aaaa");
    EXPECT_EQ(pi, (std::vector<int>{0, 1, 2, 3}));
}

TEST(KMP, PrefixFunctionAllDiff) {
    auto pi = kmp::prefix_function("abcd");
    EXPECT_EQ(pi, (std::vector<int>{0, 0, 0, 0}));
}

TEST(KMP, PrefixFunctionAbaab) {
    // "abaab": pi = {0,0,1,1,2}
    auto pi = kmp::prefix_function("abaab");
    EXPECT_EQ(pi, (std::vector<int>{0, 0, 1, 1, 2}));
}

TEST(KMP, PrefixFunctionAbcabcabc) {
    auto pi = kmp::prefix_function("abcabcabc");
    EXPECT_EQ(pi, (std::vector<int>{0, 0, 0, 1, 2, 3, 4, 5, 6}));
}

TEST(KMP, PrefixFunctionAbacaba) {
    auto pi = kmp::prefix_function("abacaba");
    EXPECT_EQ(pi, (std::vector<int>{0, 0, 1, 0, 1, 2, 3}));
}

// ==================== search ====================

TEST(KMP, SearchBasic) {
    auto pos = kmp::search("abracadabra", "abra");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 7}));
}

TEST(KMP, SearchOverlapping) {
    auto pos = kmp::search("aaaa", "aa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

TEST(KMP, SearchNoMatch) {
    auto pos = kmp::search("abcdef", "xyz");
    EXPECT_TRUE(pos.empty());
}

TEST(KMP, SearchEmptyPattern) {
    auto pos = kmp::search("hello", "");
    EXPECT_TRUE(pos.empty());
}

TEST(KMP, SearchEmptyText) {
    auto pos = kmp::search("", "abc");
    EXPECT_TRUE(pos.empty());
}

TEST(KMP, SearchPatternEqualsText) {
    auto pos = kmp::search("hello", "hello");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0}));
}

TEST(KMP, SearchPatternLongerThanText) {
    auto pos = kmp::search("hi", "hello");
    EXPECT_TRUE(pos.empty());
}

TEST(KMP, SearchSingleChar) {
    auto pos = kmp::search("abacaba", "a");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4, 6}));
}

TEST(KMP, SearchRepeated) {
    auto pos = kmp::search("ababababab", "ab");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4, 6, 8}));
}

TEST(KMP, SearchAllSame) {
    auto pos = kmp::search("aaaaa", "aaa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

TEST(KMP, SearchAtEnd) {
    auto pos = kmp::search("xxxxxabc", "abc");
    EXPECT_EQ(pos, (std::vector<std::size_t>{5}));
}

// ==================== count_occurrences ====================

TEST(KMP, CountOverlapping) {
    EXPECT_EQ(kmp::count_occurrences("aaaa", "aa"), 3u);
}

TEST(KMP, CountNone) {
    EXPECT_EQ(kmp::count_occurrences("abcdef", "xyz"), 0u);
}

TEST(KMP, CountMultiple) {
    EXPECT_EQ(kmp::count_occurrences("abracadabra", "abra"), 2u);
}

// ==================== borders ====================

TEST(KMP, BordersAbacaba) {
    auto b = kmp::borders("abacaba");
    EXPECT_EQ(b, (std::vector<std::size_t>{1, 3}));  // "a", "aba"
}

TEST(KMP, BordersAbaab) {
    auto b = kmp::borders("abaab");
    EXPECT_EQ(b, (std::vector<std::size_t>{2}));  // "ab"
}

TEST(KMP, BordersNoRepeat) {
    auto b = kmp::borders("abcde");
    EXPECT_TRUE(b.empty());
}

TEST(KMP, BordersAllSame) {
    auto b = kmp::borders("aaaa");
    EXPECT_EQ(b, (std::vector<std::size_t>{1, 2, 3}));
}

// ==================== period ====================

TEST(KMP, PeriodAbab) {
    EXPECT_EQ(kmp::period("ababab"), 2u);
}

TEST(KMP, PeriodAbc) {
    EXPECT_EQ(kmp::period("abcabc"), 3u);
}

TEST(KMP, PeriodNoPeriod) {
    EXPECT_EQ(kmp::period("abcde"), 5u);
}

TEST(KMP, PeriodAllSame) {
    EXPECT_EQ(kmp::period("aaaa"), 1u);
}

TEST(KMP, PeriodSingle) {
    EXPECT_EQ(kmp::period("a"), 1u);
}

