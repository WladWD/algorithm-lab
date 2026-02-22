#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace ds::trie {

/**
 * @brief Trie (prefix tree) data structure for efficient string operations.
 *
 * Supports insertion, lookup, deletion, and prefix-based queries over a set of
 * strings drawn from a fixed alphabet (lowercase English letters by default).
 *
 * All operations are proportional to the length of the key, independent of the
 * number of stored strings.
 */
class Trie {
public:
    static constexpr int ALPHABET_SIZE = 26;

    Trie();

    /**
     * @brief Insert a word into the trie.
     * @param word The string to insert.
     *
     * If the word already exists, this is a no-op (count is not incremented).
     */
    void insert(std::string_view word);

    /**
     * @brief Check whether a word exists in the trie.
     * @param word The string to search for.
     * @return true if the exact word was previously inserted.
     */
    [[nodiscard]] bool search(std::string_view word) const;

    /**
     * @brief Check whether any inserted word starts with the given prefix.
     * @param prefix The prefix to check.
     * @return true if at least one word in the trie starts with prefix.
     */
    [[nodiscard]] bool starts_with(std::string_view prefix) const;

    /**
     * @brief Remove a word from the trie.
     * @param word The string to remove.
     * @return true if the word was found and removed, false otherwise.
     *
     * Only removes the exact word; shared prefixes remain intact.
     */
    bool erase(std::string_view word);

    /**
     * @brief Count the number of inserted words that share the given prefix.
     * @param prefix The prefix to count.
     * @return Number of distinct words starting with prefix.
     */
    [[nodiscard]] int count_with_prefix(std::string_view prefix) const;

    /**
     * @brief Return all inserted words that share the given prefix.
     * @param prefix The prefix to search for.
     * @return Sorted vector of matching words.
     */
    [[nodiscard]] std::vector<std::string> words_with_prefix(std::string_view prefix) const;

    /**
     * @brief Find the longest prefix of the given string that is a word in the trie.
     * @param word The string to query.
     * @return The longest prefix that was inserted, or "" if none.
     */
    [[nodiscard]] std::string longest_prefix_of(std::string_view word) const;

    /**
     * @brief Return the total number of distinct words stored.
     * @return Number of words.
     */
    [[nodiscard]] int size() const;

    /**
     * @brief Check whether the trie is empty.
     * @return true if no words are stored.
     */
    [[nodiscard]] bool empty() const;

    /**
     * @brief Remove all words and reset to initial state.
     */
    void clear();

private:
    struct Node {
        int children[ALPHABET_SIZE];
        int prefix_count;   // Number of words passing through this node
        bool is_end;         // True if a word ends at this node

        Node();
    };

    std::vector<Node> nodes_;
    int word_count_;

    /**
     * @brief Allocate a new trie node.
     * @return Index of the newly created node.
     */
    int new_node();

    /**
     * @brief Convert a character to an index in [0, ALPHABET_SIZE).
     * @param c The character to convert.
     * @return Index, or -1 if invalid.
     */
    static int char_to_idx(char c);

    /**
     * @brief Traverse the trie following the given key.
     * @param key The string to follow.
     * @return Index of the node at the end of key, or -1 if path doesn't exist.
     */
    [[nodiscard]] int traverse(std::string_view key) const;

    /**
     * @brief Collect all words reachable from a given node.
     * @param node_idx Current node index.
     * @param current Current prefix being built.
     * @param result Output vector of collected words.
     */
    void collect(int node_idx, std::string& current,
                 std::vector<std::string>& result) const;
};

} // namespace ds::trie

