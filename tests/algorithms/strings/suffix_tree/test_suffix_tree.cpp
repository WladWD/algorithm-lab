#include <algorithms/strings/suffix_tree/suffix_tree.h>

#include <algorithm>
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace st = algorithms::strings::suffix_tree;

// ──────────────────────────── helper ──────────────────────────────────────

static std::vector<int> search_brute(std::string_view text, std::string_view pat) {
    std::vector<int> res;
    if (pat.empty()) return res;
    for (std::size_t i = 0; i + pat.size() <= text.size(); ++i)
        if (text.substr(i, pat.size()) == pat) res.push_back(i);
    return res;
}

// ──────────────────────────── contains ────────────────────────────────────

TEST(SuffixTree, ContainsSubstrings) {
    st::SuffixTree t("abcabc");
    EXPECT_TRUE(t.contains("a"));
    EXPECT_TRUE(t.contains("ab"));
    EXPECT_TRUE(t.contains("abc"));
    EXPECT_TRUE(t.contains("abcabc"));
    EXPECT_TRUE(t.contains("bc"));
    EXPECT_TRUE(t.contains("ca"));
    EXPECT_TRUE(t.contains("cabc"));
}

TEST(SuffixTree, ContainsMissing) {
    st::SuffixTree t("abcabc");
    EXPECT_FALSE(t.contains("d"));
    EXPECT_FALSE(t.contains("abcd"));
}

TEST(SuffixTree, ContainsEmpty) {
    st::SuffixTree t("hello");
    EXPECT_TRUE(t.contains(""));
}

TEST(SuffixTree, ContainsSingleChar) {
    st::SuffixTree t("a");
    EXPECT_TRUE(t.contains("a"));
    EXPECT_FALSE(t.contains("b"));
    EXPECT_FALSE(t.contains("aa"));
}

TEST(SuffixTree, ContainsAllSame) {
    st::SuffixTree t("aaaa");
    EXPECT_TRUE(t.contains("a"));
    EXPECT_TRUE(t.contains("aa"));
    EXPECT_TRUE(t.contains("aaaa"));
    EXPECT_FALSE(t.contains("aaaaa"));
}

TEST(SuffixTree, ContainsBanana) {
    st::SuffixTree t("banana");
    EXPECT_TRUE(t.contains("ana"));
    EXPECT_TRUE(t.contains("nana"));
    EXPECT_FALSE(t.contains("bana2"));
}

// ──────────────────────────── search ──────────────────────────────────────

TEST(SuffixTree, SearchBasic) {
    st::SuffixTree t("abracadabra");
    auto pos = t.search("abra");
    EXPECT_EQ(pos, (std::vector<int>{0, 7}));
}

TEST(SuffixTree, SearchOverlapping) {
    st::SuffixTree t("aaaa");
    auto pos = t.search("aa");
    EXPECT_EQ(pos, (std::vector<int>{0, 1, 2}));
}

TEST(SuffixTree, SearchMissing) {
    st::SuffixTree t("abcdef");
    EXPECT_TRUE(t.search("xyz").empty());
}

TEST(SuffixTree, SearchEmptyPattern) {
    st::SuffixTree t("hello");
    EXPECT_TRUE(t.search("").empty());
}

TEST(SuffixTree, SearchSingleChar) {
    st::SuffixTree t("abacaba");
    auto pos = t.search("a");
    EXPECT_EQ(pos, (std::vector<int>{0, 2, 4, 6}));
}

TEST(SuffixTree, SearchPatternEqualsText) {
    st::SuffixTree t("hello");
    auto pos = t.search("hello");
    EXPECT_EQ(pos, (std::vector<int>{0}));
}

TEST(SuffixTree, SearchPatternLonger) {
    st::SuffixTree t("hi");
    EXPECT_TRUE(t.search("hello").empty());
}

TEST(SuffixTree, SearchAgainstBrute) {
    std::string text = "mississippi";
    st::SuffixTree t(text);
    for (const char* pat : {"i", "ss", "issi", "ippi", "xyz", "mississippi", "p"}) {
        auto ours  = t.search(pat);
        auto brute = search_brute(text, pat);
        EXPECT_EQ(ours, brute) << "pat=" << pat;
    }
}

// ──────────────────────────── count_occurrences ───────────────────────────

TEST(SuffixTree, CountBanana) {
    st::SuffixTree t("banana");
    EXPECT_EQ(t.count_occurrences("a"),   3);
    EXPECT_EQ(t.count_occurrences("an"),  2);
    EXPECT_EQ(t.count_occurrences("na"),  2);
    EXPECT_EQ(t.count_occurrences("b"),   1);
    EXPECT_EQ(t.count_occurrences("xyz"), 0);
}

TEST(SuffixTree, CountAbab) {
    st::SuffixTree t("abab");
    EXPECT_EQ(t.count_occurrences("ab"),  2);
    EXPECT_EQ(t.count_occurrences("b"),   2);
    EXPECT_EQ(t.count_occurrences("aba"), 1);
}

// ──────────────────────────── num_nodes ───────────────────────────────────

TEST(SuffixTree, NumNodesBounded) {
    // Suffix tree on s + sentinel has at most 2*(n+1) nodes.
    for (const char* s : {"a", "ab", "abab", "aaaa", "banana", "mississippi"}) {
        st::SuffixTree t(s);
        std::size_t n = std::string_view(s).size() + 1; // +1 for sentinel
        EXPECT_LE(t.num_nodes(), 2 * n + 1) << "s=" << s;
    }
}

// ──────────────────────────── cross-validation ────────────────────────────

TEST(SuffixTree, AgreesWithSuffixArray) {
    // Verify search results match brute force for a range of strings/patterns.
    const std::string texts[] = {"abcabcabc", "aaabbb", "aababc"};
    const std::string pats[]  = {"a", "ab", "abc", "bc", "xyz", "aa"};
    for (const auto& text : texts) {
        st::SuffixTree t(text);
        for (const auto& pat : pats) {
            EXPECT_EQ(t.search(pat), search_brute(text, pat))
                << "text=" << text << " pat=" << pat;
        }
    }
}
