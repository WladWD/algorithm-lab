#include <algorithms/strings/suffix_automaton/suffix_automaton.h>

#include <gtest/gtest.h>
#include <string>

namespace sam = algorithms::strings::suffix_automaton;

// ──────────────────────────── contains ────────────────────────────────────

TEST(SuffixAutomaton, ContainsAll) {
    sam::SuffixAutomaton sa("abcabc");
    EXPECT_TRUE(sa.contains("a"));
    EXPECT_TRUE(sa.contains("ab"));
    EXPECT_TRUE(sa.contains("abc"));
    EXPECT_TRUE(sa.contains("abcabc"));
    EXPECT_TRUE(sa.contains("bc"));
    EXPECT_TRUE(sa.contains("ca"));
    EXPECT_TRUE(sa.contains("cabc"));
}

TEST(SuffixAutomaton, ContainsMissing) {
    sam::SuffixAutomaton sa("abcabc");
    EXPECT_FALSE(sa.contains("d"));
    EXPECT_FALSE(sa.contains("abcd"));
    EXPECT_FALSE(sa.contains("abcabcd"));
}

TEST(SuffixAutomaton, ContainsSingle) {
    sam::SuffixAutomaton sa("a");
    EXPECT_TRUE(sa.contains("a"));
    EXPECT_FALSE(sa.contains("aa"));
    EXPECT_FALSE(sa.contains("b"));
}

TEST(SuffixAutomaton, ContainsAllSame) {
    sam::SuffixAutomaton sa("aaaa");
    EXPECT_TRUE(sa.contains("a"));
    EXPECT_TRUE(sa.contains("aa"));
    EXPECT_TRUE(sa.contains("aaaa"));
    EXPECT_FALSE(sa.contains("aaaaa"));
}

// ──────────────────────────── count_occurrences ───────────────────────────

TEST(SuffixAutomaton, CountBanana) {
    sam::SuffixAutomaton sa("banana");
    EXPECT_EQ(sa.count_occurrences("a"),   3);
    EXPECT_EQ(sa.count_occurrences("an"),  2);
    EXPECT_EQ(sa.count_occurrences("na"),  2);
    EXPECT_EQ(sa.count_occurrences("ana"), 2);
    EXPECT_EQ(sa.count_occurrences("b"),   1);
    EXPECT_EQ(sa.count_occurrences("x"),   0);
}

TEST(SuffixAutomaton, CountOverlapping) {
    sam::SuffixAutomaton sa("aaaa");
    EXPECT_EQ(sa.count_occurrences("aa"),  3);
    EXPECT_EQ(sa.count_occurrences("aaa"), 2);
    EXPECT_EQ(sa.count_occurrences("a"),   4);
}

TEST(SuffixAutomaton, CountAbracadabra) {
    sam::SuffixAutomaton sa("abracadabra");
    EXPECT_EQ(sa.count_occurrences("abra"), 2);
    EXPECT_EQ(sa.count_occurrences("a"),    5);
    EXPECT_EQ(sa.count_occurrences("br"),   2);
    EXPECT_EQ(sa.count_occurrences("x"),    0);
}

TEST(SuffixAutomaton, CountEmpty) {
    sam::SuffixAutomaton sa("hello");
    EXPECT_EQ(sa.count_occurrences(""), 0);
}

// ──────────────────────────── count_distinct_substrings ───────────────────

TEST(SuffixAutomaton, DistinctAllUnique) {
    // "abcd": 1+2+3+4 = 10 distinct substrings
    sam::SuffixAutomaton sa("abcd");
    EXPECT_EQ(sa.count_distinct_substrings(), 10);
}

TEST(SuffixAutomaton, DistinctAllSame) {
    // "aaaa": "a","aa","aaa","aaaa" = 4
    sam::SuffixAutomaton sa("aaaa");
    EXPECT_EQ(sa.count_distinct_substrings(), 4);
}

TEST(SuffixAutomaton, DistinctBanana) {
    // "banana" has 15 distinct substrings (n*(n+1)/2 - duplicates)
    sam::SuffixAutomaton sa("banana");
    EXPECT_EQ(sa.count_distinct_substrings(), 15);
}

TEST(SuffixAutomaton, DistinctSingle) {
    sam::SuffixAutomaton sa("a");
    EXPECT_EQ(sa.count_distinct_substrings(), 1);
}

TEST(SuffixAutomaton, DistinctAbab) {
    // "abab": total 4*5/2=10 - duplicates{a,b,ab} = 7
    sam::SuffixAutomaton sa("abab");
    EXPECT_EQ(sa.count_distinct_substrings(), 7);
}

// ──────────────────────────── num_states ──────────────────────────────────

TEST(SuffixAutomaton, NumStatesBounded) {
    // SAM has at most 2n-1 states for n >= 2
    for (const char* s : {"a", "ab", "abc", "aaaa", "banana", "mississippi"}) {
        sam::SuffixAutomaton sa(s);
        std::size_t n = std::string_view(s).size();
        EXPECT_LE(sa.num_states(), 2 * n) << "s=" << s;
        EXPECT_GE(sa.num_states(), 2u);
    }
}
