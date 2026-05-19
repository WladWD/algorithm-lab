#include <algorithms/strings/polynomial_hash/polynomial_hash.h>

#include <algorithm>
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace ph = algorithms::strings::polynomial_hash;

// ==================== PolyHash basic ====================

TEST(PolyHash, FullStringHash) {
    ph::PolyHash h("hello");
    EXPECT_GT(h.full(), 0u);
    EXPECT_EQ(h.size(), 5u);
}

TEST(PolyHash, EqualStringsHaveSameHash) {
    ph::PolyHash h1("abcde");
    ph::PolyHash h2("abcde");
    EXPECT_EQ(h1.full(), h2.full());
}

TEST(PolyHash, DifferentStringsLikelyDifferentHash) {
    ph::PolyHash h1("abcde");
    ph::PolyHash h2("abcdf");
    EXPECT_NE(h1.full(), h2.full());
}

TEST(PolyHash, SubstringQueryMatchesFullHash) {
    // hash(s[0..n-1]) should equal full()
    std::string s = "hello world";
    ph::PolyHash h(s);
    // full hash is h[n], not query(0, n-1) — they are equivalent up to mod
    EXPECT_EQ(h.query(0, s.size() - 1), h.full());
}

TEST(PolyHash, SubstringHashSingle) {
    std::string s = "abcde";
    ph::PolyHash h(s);
    // hash of a single character = its ASCII value
    for (std::size_t i = 0; i < s.size(); ++i) {
        ph::PolyHash single(s.substr(i, 1));
        EXPECT_EQ(h.query(i, i), single.full());
    }
}

TEST(PolyHash, SubstringHashEquality) {
    std::string s = "abracadabra";
    ph::PolyHash h(s);
    // s[0..3] == "abra", s[7..10] == "abra" — hashes must match
    EXPECT_EQ(h.query(0, 3), h.query(7, 10));
    // s[0..3] != s[4..7] ("abra" vs "cada")
    EXPECT_NE(h.query(0, 3), h.query(4, 7));
}

TEST(PolyHash, EmptyString) {
    ph::PolyHash h("");
    EXPECT_EQ(h.full(), 0u);
    EXPECT_EQ(h.size(), 0u);
}

// ==================== DoubleHash ====================

TEST(DoubleHash, EqualSubstringsHaveSameDoubleHash) {
    std::string s = "abracadabra";
    ph::DoubleHash dh(s);
    EXPECT_EQ(dh.query(0, 3), dh.query(7, 10));
}

TEST(DoubleHash, DifferentSubstringsLikelyDifferent) {
    std::string s = "abracadabra";
    ph::DoubleHash dh(s);
    EXPECT_NE(dh.query(0, 3), dh.query(1, 4));
}

TEST(DoubleHash, FullHash) {
    ph::DoubleHash dh("hello");
    auto [h1, h2] = dh.full();
    EXPECT_GT(h1, 0u);
    EXPECT_GT(h2, 0u);
}

// ==================== substrings_equal ====================

TEST(SubstringsEqual, EqualSubstrings) {
    ph::DoubleHash dh("abracadabra");
    EXPECT_TRUE(ph::substrings_equal(dh, 0, 3, dh, 7, 10));  // "abra" == "abra"
}

TEST(SubstringsEqual, UnequalSubstrings) {
    ph::DoubleHash dh("abracadabra");
    EXPECT_FALSE(ph::substrings_equal(dh, 0, 3, dh, 4, 7));  // "abra" != "cada"
}

TEST(SubstringsEqual, DifferentLengths) {
    ph::DoubleHash dh("abcdef");
    EXPECT_FALSE(ph::substrings_equal(dh, 0, 2, dh, 0, 3));  // "abc" != "abcd"
}

TEST(SubstringsEqual, AcrossTwoStrings) {
    ph::DoubleHash a("hello world");
    ph::DoubleHash b("say hello to");
    // "hello" in a[0..4] vs "hello" in b[4..8]
    EXPECT_TRUE(ph::substrings_equal(a, 0, 4, b, 4, 8));
    EXPECT_FALSE(ph::substrings_equal(a, 0, 4, b, 0, 4));  // "hello" vs "say h"
}

// ==================== lcp ====================

TEST(LCP, IdenticalPosition) {
    ph::DoubleHash dh("abracadabra");
    // lcp(s, i, i) = n - i
    EXPECT_EQ(ph::lcp(dh, 0, 0), 11u);
    EXPECT_EQ(ph::lcp(dh, 7, 7), 4u);
}

TEST(LCP, KnownLCP) {
    // "abracadabra": s[0..] = "abracadabra", s[7..] = "abra"
    // LCP = 4 ("abra")
    ph::DoubleHash dh("abracadabra");
    EXPECT_EQ(ph::lcp(dh, 0, 7), 4u);
}

TEST(LCP, NoCommonPrefix) {
    ph::DoubleHash dh("abcdef");
    EXPECT_EQ(ph::lcp(dh, 0, 3), 0u);  // 'a' vs 'd'
}

TEST(LCP, AllSameCharacters) {
    ph::DoubleHash dh("aaaaa");
    EXPECT_EQ(ph::lcp(dh, 0, 2), 3u);  // "aaa" vs "aaa"
    EXPECT_EQ(ph::lcp(dh, 0, 4), 1u);  // "a" (only one char remains at pos 4)
}

// ==================== find_all ====================

TEST(FindAll, BasicPattern) {
    auto pos = ph::find_all("abracadabra", "abra");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 7}));
}

TEST(FindAll, PatternAtStart) {
    auto pos = ph::find_all("aaabbb", "aaa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0}));
}

TEST(FindAll, PatternAtEnd) {
    auto pos = ph::find_all("bbbaaaa", "aaaa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{3}));
}

TEST(FindAll, OverlappingPattern) {
    auto pos = ph::find_all("aaaa", "aa");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 2}));
}

TEST(FindAll, NoOccurrence) {
    auto pos = ph::find_all("abcdef", "xyz");
    EXPECT_TRUE(pos.empty());
}

TEST(FindAll, PatternLongerThanText) {
    auto pos = ph::find_all("abc", "abcd");
    EXPECT_TRUE(pos.empty());
}

TEST(FindAll, EmptyPattern) {
    auto pos = ph::find_all("hello", "");
    EXPECT_TRUE(pos.empty());
}

TEST(FindAll, SingleChar) {
    auto pos = ph::find_all("aababc", "a");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 1, 3}));
}

TEST(FindAll, EntireText) {
    auto pos = ph::find_all("hello", "hello");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0}));
}

TEST(FindAll, RepeatedPattern) {
    auto pos = ph::find_all("ababab", "ab");
    EXPECT_EQ(pos, (std::vector<std::size_t>{0, 2, 4}));
}

