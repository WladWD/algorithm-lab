#include <algorithms/strings/aho_corasick/aho_corasick.h>

#include <algorithm>
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace ac = algorithms::strings::aho_corasick;

// Helper: extract start positions for a specific pattern id
static std::vector<std::size_t> starts_for(const std::vector<ac::Match>& matches,
                                            std::size_t pid,
                                            const std::vector<std::string>& patterns) {
    std::vector<std::size_t> result;
    for (const auto& m : matches) {
        if (m.pattern_id == pid) {
            result.push_back(m.end_pos + 1 - patterns[pid].size());
        }
    }
    return result;
}

// ==================== Basic search ====================

TEST(AhoCorasick, OriginalPaper) {
    // Classic example from Aho & Corasick 1975
    ac::AhoCorasick ac;
    ac.add_pattern("he");    // 0
    ac.add_pattern("she");   // 1
    ac.add_pattern("his");   // 2
    ac.add_pattern("hers");  // 3
    ac.build();

    auto m = ac.search("ushers");
    // Expected: "he" at end=2, "she" at end=2, "hers" at end=5
    bool found_he   = false, found_she  = false, found_hers = false;
    for (const auto& match : m) {
        if (match.pattern_id == 0 && match.end_pos == 3) found_he   = true;
        if (match.pattern_id == 1 && match.end_pos == 3) found_she  = true;
        if (match.pattern_id == 3 && match.end_pos == 5) found_hers = true;
    }
    EXPECT_TRUE(found_he);
    EXPECT_TRUE(found_she);
    EXPECT_TRUE(found_hers);
}

TEST(AhoCorasick, SinglePattern) {
    ac::AhoCorasick ac;
    ac.add_pattern("abra");
    ac.build();
    auto m = ac.search("abracadabra");
    ASSERT_EQ(m.size(), 2u);
    EXPECT_EQ(m[0].end_pos, 3u);
    EXPECT_EQ(m[1].end_pos, 10u);
}

TEST(AhoCorasick, NoMatch) {
    ac::AhoCorasick ac;
    ac.add_pattern("xyz");
    ac.build();
    auto m = ac.search("abcdef");
    EXPECT_TRUE(m.empty());
}

TEST(AhoCorasick, OverlappingPatterns) {
    ac::AhoCorasick ac;
    ac.add_pattern("a");    // 0
    ac.add_pattern("aa");   // 1
    ac.add_pattern("aaa");  // 2
    ac.build();
    auto m = ac.search("aaaa");
    // "a"   at pos 0,1,2,3
    // "aa"  at pos 0,1,2
    // "aaa" at pos 0,1
    std::size_t cnt_a = 0, cnt_aa = 0, cnt_aaa = 0;
    for (const auto& match : m) {
        if (match.pattern_id == 0) ++cnt_a;
        if (match.pattern_id == 1) ++cnt_aa;
        if (match.pattern_id == 2) ++cnt_aaa;
    }
    EXPECT_EQ(cnt_a,   4u);
    EXPECT_EQ(cnt_aa,  3u);
    EXPECT_EQ(cnt_aaa, 2u);
}

TEST(AhoCorasick, EmptyText) {
    ac::AhoCorasick ac;
    ac.add_pattern("abc");
    ac.build();
    EXPECT_TRUE(ac.search("").empty());
}

TEST(AhoCorasick, PatternLongerThanText) {
    ac::AhoCorasick ac;
    ac.add_pattern("hello world");
    ac.build();
    EXPECT_TRUE(ac.search("hi").empty());
}

TEST(AhoCorasick, MultiplePatternsSamePosition) {
    // "a" is a suffix of "ba"
    ac::AhoCorasick ac;
    ac.add_pattern("a");   // 0
    ac.add_pattern("ba");  // 1
    ac.build();
    auto m = ac.search("ba");
    // "a" ends at pos 1, "ba" ends at pos 1
    bool found_a = false, found_ba = false;
    for (const auto& match : m) {
        if (match.pattern_id == 0 && match.end_pos == 1) found_a  = true;
        if (match.pattern_id == 1 && match.end_pos == 1) found_ba = true;
    }
    EXPECT_TRUE(found_a);
    EXPECT_TRUE(found_ba);
}

TEST(AhoCorasick, PatternEqualsText) {
    ac::AhoCorasick ac;
    ac.add_pattern("hello");
    ac.build();
    auto m = ac.search("hello");
    ASSERT_EQ(m.size(), 1u);
    EXPECT_EQ(m[0].end_pos, 4u);
    EXPECT_EQ(m[0].pattern_id, 0u);
}

TEST(AhoCorasick, DisjointPatterns) {
    ac::AhoCorasick ac;
    ac.add_pattern("cat");   // 0
    ac.add_pattern("dog");   // 1
    ac.add_pattern("bird");  // 2
    ac.build();
    auto m = ac.search("the cat sat next to the dog and the bird flew");
    auto ps = ac.patterns();
    auto cat_pos  = starts_for(m, 0, ps);
    auto dog_pos  = starts_for(m, 1, ps);
    auto bird_pos = starts_for(m, 2, ps);
    EXPECT_EQ(cat_pos,  (std::vector<std::size_t>{4}));
    EXPECT_EQ(dog_pos,  (std::vector<std::size_t>{23}));
    EXPECT_EQ(bird_pos, (std::vector<std::size_t>{35}));
}

TEST(AhoCorasick, RepeatedPattern) {
    ac::AhoCorasick ac;
    ac.add_pattern("ab");
    ac.build();
    auto m = ac.search("ababababab");
    ASSERT_EQ(m.size(), 5u);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(m[i].end_pos, 2 * i + 1);
    }
}

TEST(AhoCorasick, CaseInsensitive) {
    // Aho-Corasick operates on bytes; just verify lowercase works
    ac::AhoCorasick ac;
    ac.add_pattern("test");
    ac.build();
    auto m = ac.search("this is a test of testing");
    EXPECT_EQ(m.size(), 2u);  // "test" at pos 10, "test"ing at pos 17
}

TEST(AhoCorasick, BinaryData) {
    // Works on arbitrary byte strings
    ac::AhoCorasick ac;
    std::string pat = {'\x01', '\x02', '\x03'};
    ac.add_pattern(pat);
    ac.build();
    std::string text = {'\x00', '\x01', '\x02', '\x03', '\x04'};
    auto m = ac.search(text);
    ASSERT_EQ(m.size(), 1u);
    EXPECT_EQ(m[0].end_pos, 3u);
}

TEST(AhoCorasick, LargeText) {
    // Build automaton with 3 patterns, search large repeated text
    ac::AhoCorasick ac;
    ac.add_pattern("ab");  // 0
    ac.add_pattern("cd");  // 1
    ac.add_pattern("abcd"); // 2
    ac.build();

    std::string text;
    for (int i = 0; i < 500; ++i) text += "abcd";

    auto m = ac.search(text);
    std::size_t cnt_ab = 0, cnt_cd = 0, cnt_abcd = 0;
    for (const auto& match : m) {
        if (match.pattern_id == 0) ++cnt_ab;
        if (match.pattern_id == 1) ++cnt_cd;
        if (match.pattern_id == 2) ++cnt_abcd;
    }
    EXPECT_EQ(cnt_ab,   500u);
    EXPECT_EQ(cnt_cd,   500u);
    EXPECT_EQ(cnt_abcd, 500u);
}

