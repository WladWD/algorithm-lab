#include <algorithms/strings/suffix_array/suffix_array.h>

#include <algorithm>
#include <gtest/gtest.h>
#include <numeric>
#include <string>
#include <vector>

namespace sa = algorithms::strings::suffix_array;

// ──────────────────────────── helpers ─────────────────────────────────────

// Brute-force suffix array for cross-validation.
static std::vector<int> sa_brute(std::string_view s) {
    int n = s.size();
    std::vector<int> idx(n);
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&](int a, int b) {
        return s.substr(a) < s.substr(b);
    });
    return idx;
}

// Brute-force search.
static std::vector<int> search_brute(std::string_view text, std::string_view pat) {
    std::vector<int> res;
    if (pat.empty()) return res;
    for (std::size_t i = 0; i + pat.size() <= text.size(); ++i)
        if (text.substr(i, pat.size()) == pat) res.push_back(i);
    return res;
}

// ──────────────────────────── build ───────────────────────────────────────

TEST(SuffixArray, Empty) {
    EXPECT_TRUE(sa::build("").empty());
}

TEST(SuffixArray, SingleChar) {
    auto result = sa::build("a");
    EXPECT_EQ(result, (std::vector<int>{0}));
}

TEST(SuffixArray, Banana) {
    // "banana" → suffixes sorted: "a","ana","anana","banana","na","nana"
    auto result = sa::build("banana");
    EXPECT_EQ(result, (std::vector<int>{5, 3, 1, 0, 4, 2}));
}

TEST(SuffixArray, AllSame) {
    // "aaaa" → sorted: "a","aa","aaa","aaaa"
    auto result = sa::build("aaaa");
    EXPECT_EQ(result, (std::vector<int>{3, 2, 1, 0}));
}

TEST(SuffixArray, AllDistinct) {
    auto result = sa::build("abcd");
    EXPECT_EQ(result, (std::vector<int>{0, 1, 2, 3}));
}

TEST(SuffixArray, AgainstBrute) {
    for (const char* s : {"mississippi", "abababab", "aabbaabb", "zzzzz", "abcabc"}) {
        EXPECT_EQ(sa::build(s), sa_brute(s)) << "text=" << s;
    }
}

// ──────────────────────────── build_lcp ───────────────────────────────────

TEST(SuffixArray, LcpBanana) {
    std::string s = "banana";
    auto arr = sa::build(s);
    auto lcp = sa::build_lcp(s, arr);
    // SA=[5,3,1,0,4,2]: "a","ana","anana","banana","na","nana"
    // LCP:              0,   1,    3,       0,       0,   2
    EXPECT_EQ(lcp, (std::vector<int>{0, 1, 3, 0, 0, 2}));
}

TEST(SuffixArray, LcpAllSame) {
    std::string s = "aaaa";
    auto arr = sa::build(s);
    auto lcp = sa::build_lcp(s, arr);
    EXPECT_EQ(lcp, (std::vector<int>{0, 1, 2, 3}));
}

TEST(SuffixArray, LcpAllDistinct) {
    std::string s = "abcd";
    auto arr = sa::build(s);
    auto lcp = sa::build_lcp(s, arr);
    EXPECT_EQ(lcp, (std::vector<int>{0, 0, 0, 0}));
}

TEST(SuffixArray, LcpNonNegative) {
    std::string s = "mississippi";
    auto arr = sa::build(s);
    auto lcp = sa::build_lcp(s, arr);
    EXPECT_EQ(lcp.size(), s.size());
    EXPECT_EQ(lcp[0], 0);
    for (int v : lcp) EXPECT_GE(v, 0);
}

// ──────────────────────────── search ──────────────────────────────────────

TEST(SuffixArray, SearchBasic) {
    std::string text = "abracadabra";
    auto arr = sa::build(text);
    auto pos  = sa::search(text, "abra", arr);
    EXPECT_EQ(pos, (std::vector<int>{0, 7}));
}

TEST(SuffixArray, SearchMissing) {
    std::string text = "abcdef";
    auto arr = sa::build(text);
    EXPECT_TRUE(sa::search(text, "xyz", arr).empty());
}

TEST(SuffixArray, SearchEmptyPattern) {
    std::string text = "hello";
    auto arr = sa::build(text);
    EXPECT_TRUE(sa::search(text, "", arr).empty());
}

TEST(SuffixArray, SearchOverlapping) {
    std::string text = "aaaa";
    auto arr = sa::build(text);
    auto pos  = sa::search(text, "aa", arr);
    EXPECT_EQ(pos, (std::vector<int>{0, 1, 2}));
}

TEST(SuffixArray, SearchAgainstBrute) {
    std::string text = "mississippi";
    auto arr = sa::build(text);
    for (const char* pat : {"i", "ss", "issi", "ippi", "xyz", "mississippi"}) {
        auto ours  = sa::search(text, pat, arr);
        auto brute = search_brute(text, pat);
        EXPECT_EQ(ours, brute) << "pat=" << pat;
    }
}

// ──────────────────────────── count ───────────────────────────────────────

TEST(SuffixArray, CountBanana) {
    std::string text = "banana";
    auto arr = sa::build(text);
    EXPECT_EQ(sa::count_occurrences(text, "an", arr), 2);
    EXPECT_EQ(sa::count_occurrences(text, "na", arr), 2);
    EXPECT_EQ(sa::count_occurrences(text, "a",  arr), 3);
    EXPECT_EQ(sa::count_occurrences(text, "x",  arr), 0);
}
