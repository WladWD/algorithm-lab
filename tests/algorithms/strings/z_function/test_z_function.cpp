#include <algorithms/strings/z_function/z_function.h>

#include <gtest/gtest.h>
#include <vector>

namespace zf = algorithms::strings::z_function;

// ==================== z_function ====================

TEST(ZFunction, Empty) {
    auto z = zf::z_function("");
    EXPECT_TRUE(z.empty());
}

TEST(ZFunction, Single) {
    auto z = zf::z_function("a");
    EXPECT_EQ(z, (std::vector<std::size_t>{0}));
}

TEST(ZFunction, AllSame) {
    auto z = zf::z_function("aaaa");
    EXPECT_EQ(z, (std::vector<std::size_t>{0, 3, 2, 1}));
}

TEST(ZFunction, AllDiff) {
    auto z = zf::z_function("abcd");
    EXPECT_EQ(z, (std::vector<std::size_t>{0, 0, 0, 0}));
}

TEST(ZFunction, AabXaa) {
    // "aabxaa": z = {0,1,0,0,2,1}
    auto z = zf::z_function("aabxaa");
    EXPECT_EQ(z, (std::vector<std::size_t>{0, 1, 0, 0, 2, 1}));
}

TEST(ZFunction, AbacAba) {
    // "abacaba": z = {0,0,1,0,3,0,1}
    auto z = zf::z_function("abacaba");
    EXPECT_EQ(z, (std::vector<std::size_t>{0, 0, 1, 0, 3, 0, 1}));
}

TEST(ZFunction, AbcAbcAbc) {
    // "abcabcabc": each "abc" at positions 3,6 matches prefix of length 6,3
    auto z = zf::z_function("abcabcabc");
    EXPECT_EQ(z, (std::vector<std::size_t>{0, 0, 0, 6, 0, 0, 3, 0, 0}));
}

// ==================== search ====================

TEST(ZFunction, SearchBasic) {
    auto pos = zf::search("abracadabra", "abra");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 7}));
}

TEST(ZFunction, SearchOverlapping) {
    auto pos = zf::search("aaaa", "aa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

TEST(ZFunction, SearchNoMatch) {
    auto pos = zf::search("abcdef", "xyz");
    EXPECT_TRUE(pos.empty());
}

TEST(ZFunction, SearchEmptyPattern) {
    auto pos = zf::search("hello", "");
    EXPECT_TRUE(pos.empty());
}

TEST(ZFunction, SearchPatternEqualsText) {
    auto pos = zf::search("hello", "hello");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0}));
}

TEST(ZFunction, SearchPatternLonger) {
    auto pos = zf::search("hi", "hello");
    EXPECT_TRUE(pos.empty());
}

TEST(ZFunction, SearchSingleChar) {
    auto pos = zf::search("abacaba", "a");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4, 6}));
}

TEST(ZFunction, SearchRepeated) {
    auto pos = zf::search("ababababab", "ab");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4, 6, 8}));
}

TEST(ZFunction, SearchAllSame) {
    auto pos = zf::search("aaaaa", "aaa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

// ==================== Agreement with KMP ====================

TEST(ZFunction, MatchesKMPResults) {
    // Large text / pattern — verify same positions as KMP would give
    std::string text = "abcabcabcabcabc";
    std::string pat  = "abcabc";
    auto pos = zf::search(text, pat);
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 3, 6, 9}));
}

