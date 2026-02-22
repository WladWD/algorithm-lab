#include <data_structures/trie/trie.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

namespace {
using ds::trie::Trie;

// ---- Construction & Empty State ----

TEST(Trie, EmptyOnConstruction) {
    Trie t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
}

TEST(Trie, SearchInEmpty) {
    Trie t;
    EXPECT_FALSE(t.search("hello"));
    EXPECT_FALSE(t.starts_with("h"));
}

// ---- Insert & Search ----

TEST(Trie, InsertAndSearch) {
    Trie t;
    t.insert("apple");
    EXPECT_TRUE(t.search("apple"));
    EXPECT_FALSE(t.search("app"));
    EXPECT_FALSE(t.search("apples"));
    EXPECT_EQ(t.size(), 1);
}

TEST(Trie, InsertMultipleWords) {
    Trie t;
    t.insert("apple");
    t.insert("app");
    t.insert("banana");
    t.insert("band");
    t.insert("ban");

    EXPECT_TRUE(t.search("apple"));
    EXPECT_TRUE(t.search("app"));
    EXPECT_TRUE(t.search("banana"));
    EXPECT_TRUE(t.search("band"));
    EXPECT_TRUE(t.search("ban"));
    EXPECT_FALSE(t.search("ba"));
    EXPECT_FALSE(t.search("banan"));
    EXPECT_EQ(t.size(), 5);
}

TEST(Trie, InsertDuplicateIsNoop) {
    Trie t;
    t.insert("hello");
    t.insert("hello");
    t.insert("hello");
    EXPECT_EQ(t.size(), 1);
    EXPECT_TRUE(t.search("hello"));
}

TEST(Trie, InsertSingleCharacter) {
    Trie t;
    t.insert("a");
    EXPECT_TRUE(t.search("a"));
    EXPECT_FALSE(t.search("b"));
    EXPECT_EQ(t.size(), 1);
}

TEST(Trie, InsertEmptyString) {
    Trie t;
    t.insert("");
    // Empty string is a valid word at the root
    EXPECT_TRUE(t.search(""));
    EXPECT_EQ(t.size(), 1);
}

// ---- Prefix Queries ----

TEST(Trie, StartsWith) {
    Trie t;
    t.insert("apple");
    t.insert("app");
    t.insert("application");

    EXPECT_TRUE(t.starts_with("a"));
    EXPECT_TRUE(t.starts_with("ap"));
    EXPECT_TRUE(t.starts_with("app"));
    EXPECT_TRUE(t.starts_with("appl"));
    EXPECT_TRUE(t.starts_with("apple"));
    EXPECT_TRUE(t.starts_with("appli"));
    EXPECT_FALSE(t.starts_with("b"));
    EXPECT_FALSE(t.starts_with("applz"));
}

TEST(Trie, StartsWithEmptyPrefix) {
    Trie t;
    t.insert("hello");
    // Empty prefix matches everything
    EXPECT_TRUE(t.starts_with(""));
}

TEST(Trie, CountWithPrefix) {
    Trie t;
    t.insert("apple");
    t.insert("app");
    t.insert("application");
    t.insert("banana");

    EXPECT_EQ(t.count_with_prefix("app"), 3);
    EXPECT_EQ(t.count_with_prefix("apple"), 1);
    EXPECT_EQ(t.count_with_prefix("appl"), 2);  // apple, application
    EXPECT_EQ(t.count_with_prefix("ban"), 1);
    EXPECT_EQ(t.count_with_prefix("xyz"), 0);
}

TEST(Trie, CountWithEmptyPrefix) {
    Trie t;
    t.insert("a");
    t.insert("b");
    t.insert("c");
    // Empty prefix → count all words
    // prefix_count at root = sum of all words passing through root
    EXPECT_EQ(t.count_with_prefix(""), 3);
}

TEST(Trie, WordsWithPrefix) {
    Trie t;
    t.insert("apple");
    t.insert("app");
    t.insert("application");
    t.insert("banana");
    t.insert("band");

    auto result = t.words_with_prefix("app");
    std::vector<std::string> expected = {"app", "apple", "application"};
    EXPECT_EQ(result, expected);

    result = t.words_with_prefix("ban");
    expected = {"banana", "band"};
    EXPECT_EQ(result, expected);

    result = t.words_with_prefix("xyz");
    EXPECT_TRUE(result.empty());
}

TEST(Trie, WordsWithPrefixEmptyPrefix) {
    Trie t;
    t.insert("cat");
    t.insert("car");
    t.insert("bat");

    auto result = t.words_with_prefix("");
    std::vector<std::string> expected = {"bat", "car", "cat"};
    EXPECT_EQ(result, expected);
}

// ---- Longest Prefix Of ----

TEST(Trie, LongestPrefixOf) {
    Trie t;
    t.insert("a");
    t.insert("app");
    t.insert("apple");

    EXPECT_EQ(t.longest_prefix_of("application"), "app");
    EXPECT_EQ(t.longest_prefix_of("apple"), "apple");
    EXPECT_EQ(t.longest_prefix_of("applepie"), "apple");
    EXPECT_EQ(t.longest_prefix_of("a"), "a");
    EXPECT_EQ(t.longest_prefix_of("ant"), "a");
}

TEST(Trie, LongestPrefixOfNoMatch) {
    Trie t;
    t.insert("hello");
    EXPECT_EQ(t.longest_prefix_of("world"), "");
    EXPECT_EQ(t.longest_prefix_of(""), "");
}

// ---- Erase ----

TEST(Trie, EraseExistingWord) {
    Trie t;
    t.insert("apple");
    t.insert("app");

    EXPECT_TRUE(t.erase("apple"));
    EXPECT_FALSE(t.search("apple"));
    EXPECT_TRUE(t.search("app"));  // prefix still exists
    EXPECT_EQ(t.size(), 1);
}

TEST(Trie, EraseNonexistentWord) {
    Trie t;
    t.insert("apple");
    EXPECT_FALSE(t.erase("app"));
    EXPECT_FALSE(t.erase("banana"));
    EXPECT_EQ(t.size(), 1);
}

TEST(Trie, EraseAllWords) {
    Trie t;
    t.insert("a");
    t.insert("b");
    t.insert("c");

    EXPECT_TRUE(t.erase("a"));
    EXPECT_TRUE(t.erase("b"));
    EXPECT_TRUE(t.erase("c"));

    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
    EXPECT_FALSE(t.search("a"));
}

TEST(Trie, ErasePrefixDoesNotAffectLongerWord) {
    Trie t;
    t.insert("app");
    t.insert("apple");

    EXPECT_TRUE(t.erase("app"));
    EXPECT_FALSE(t.search("app"));
    EXPECT_TRUE(t.search("apple"));
    EXPECT_TRUE(t.starts_with("app"));  // path still exists for "apple"
    EXPECT_EQ(t.size(), 1);
}

TEST(Trie, EraseLongerWordDoesNotAffectPrefix) {
    Trie t;
    t.insert("app");
    t.insert("apple");

    EXPECT_TRUE(t.erase("apple"));
    EXPECT_TRUE(t.search("app"));
    EXPECT_FALSE(t.search("apple"));
    EXPECT_EQ(t.size(), 1);
}

TEST(Trie, EraseAndReinsert) {
    Trie t;
    t.insert("hello");
    EXPECT_TRUE(t.erase("hello"));
    EXPECT_FALSE(t.search("hello"));

    t.insert("hello");
    EXPECT_TRUE(t.search("hello"));
    EXPECT_EQ(t.size(), 1);
}

TEST(Trie, EraseDuplicate) {
    Trie t;
    t.insert("word");
    EXPECT_TRUE(t.erase("word"));
    EXPECT_FALSE(t.erase("word")); // already gone
    EXPECT_EQ(t.size(), 0);
}

// ---- Clear ----

TEST(Trie, Clear) {
    Trie t;
    t.insert("one");
    t.insert("two");
    t.insert("three");
    EXPECT_EQ(t.size(), 3);

    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
    EXPECT_FALSE(t.search("one"));
    EXPECT_FALSE(t.starts_with("t"));
}

TEST(Trie, ClearAndReuse) {
    Trie t;
    t.insert("abc");
    t.clear();

    t.insert("xyz");
    EXPECT_TRUE(t.search("xyz"));
    EXPECT_FALSE(t.search("abc"));
    EXPECT_EQ(t.size(), 1);
}

// ---- Edge Cases ----

TEST(Trie, AllSameCharacters) {
    Trie t;
    t.insert("aaa");
    t.insert("aa");
    t.insert("a");

    EXPECT_TRUE(t.search("a"));
    EXPECT_TRUE(t.search("aa"));
    EXPECT_TRUE(t.search("aaa"));
    EXPECT_FALSE(t.search("aaaa"));
    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.count_with_prefix("a"), 3);
    EXPECT_EQ(t.count_with_prefix("aa"), 2);
    EXPECT_EQ(t.count_with_prefix("aaa"), 1);
}

TEST(Trie, SingleLetterWords) {
    Trie t;
    for (char c = 'a'; c <= 'z'; ++c) {
        t.insert(std::string(1, c));
    }
    EXPECT_EQ(t.size(), 26);
    for (char c = 'a'; c <= 'z'; ++c) {
        EXPECT_TRUE(t.search(std::string(1, c)));
    }
}

TEST(Trie, LongWord) {
    Trie t;
    std::string long_word(1000, 'a');
    t.insert(long_word);
    EXPECT_TRUE(t.search(long_word));
    EXPECT_TRUE(t.starts_with(long_word.substr(0, 500)));
    EXPECT_EQ(t.size(), 1);
}

TEST(Trie, OverlappingPrefixes) {
    Trie t;
    t.insert("the");
    t.insert("there");
    t.insert("their");
    t.insert("them");
    t.insert("then");

    EXPECT_EQ(t.count_with_prefix("the"), 5);
    EXPECT_EQ(t.count_with_prefix("ther"), 1);
    EXPECT_EQ(t.count_with_prefix("thei"), 1);

    auto words = t.words_with_prefix("the");
    EXPECT_EQ(words.size(), 5u);
    // Should be sorted
    EXPECT_TRUE(std::is_sorted(words.begin(), words.end()));
}

// ---- Stress / Larger Input ----

TEST(Trie, ManyWordsInsertAndSearch) {
    Trie t;
    // Insert words: "word0", "word1", ..., "word999"
    // But since we only support [a-z], use: "worda", "wordb", ..., prefix + suffix
    const int N = 500;
    std::vector<std::string> words;
    words.reserve(N);
    for (int i = 0; i < N; ++i) {
        std::string w = "prefix";
        // Encode i in base-26
        int val = i;
        do {
            w.push_back(static_cast<char>('a' + val % 26));
            val /= 26;
        } while (val > 0);
        words.push_back(w);
    }

    for (const auto& w : words) {
        t.insert(w);
    }

    EXPECT_EQ(t.size(), N);

    for (const auto& w : words) {
        EXPECT_TRUE(t.search(w));
    }

    EXPECT_TRUE(t.starts_with("prefix"));
    EXPECT_EQ(t.count_with_prefix("prefix"), N);
}

TEST(Trie, EraseUpdatesCountWithPrefix) {
    Trie t;
    t.insert("abc");
    t.insert("abd");
    t.insert("aef");

    EXPECT_EQ(t.count_with_prefix("a"), 3);
    EXPECT_EQ(t.count_with_prefix("ab"), 2);

    t.erase("abc");
    EXPECT_EQ(t.count_with_prefix("a"), 2);
    EXPECT_EQ(t.count_with_prefix("ab"), 1);
    EXPECT_EQ(t.count_with_prefix("abc"), 0);
}

TEST(Trie, WordsWithPrefixAfterErase) {
    Trie t;
    t.insert("cat");
    t.insert("car");
    t.insert("card");

    auto words = t.words_with_prefix("ca");
    EXPECT_EQ(words.size(), 3u);

    t.erase("car");
    words = t.words_with_prefix("ca");
    std::vector<std::string> expected = {"card", "cat"};
    EXPECT_EQ(words, expected);
}

TEST(Trie, LongestPrefixOfAfterErase) {
    Trie t;
    t.insert("a");
    t.insert("ab");
    t.insert("abc");

    EXPECT_EQ(t.longest_prefix_of("abcdef"), "abc");

    t.erase("abc");
    EXPECT_EQ(t.longest_prefix_of("abcdef"), "ab");

    t.erase("ab");
    EXPECT_EQ(t.longest_prefix_of("abcdef"), "a");

    t.erase("a");
    EXPECT_EQ(t.longest_prefix_of("abcdef"), "");
}

} // namespace

