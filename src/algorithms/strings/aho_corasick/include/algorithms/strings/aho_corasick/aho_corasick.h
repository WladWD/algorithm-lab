#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace algorithms::strings::aho_corasick {

// Aho-Corasick multi-pattern matching automaton.
//
// Builds a trie of all patterns, adds failure (suffix) links (like KMP's
// prefix function), and optionally dictionary links for output chaining.
// Then runs the text through the automaton in O(n + total_matches).
//
// - Build:   O(Σ * M) where M = sum of pattern lengths, Σ = alphabet size.
// - Search:  O(n + z) where n = text length, z = number of matches.
// - Space:   O(Σ * M).

/// A single match: {text_position (end, inclusive), pattern_index}.
struct Match {
    std::size_t end_pos;     ///< 0-indexed end position (inclusive) in text
    std::size_t pattern_id;  ///< index in the order patterns were added
};

class AhoCorasick {
public:
    AhoCorasick() = default;

    /// Add a pattern. Returns its 0-based index. Must be called before build().
    std::size_t add_pattern(std::string_view pattern);

    /// Build failure links and dictionary links. Call once after all add_pattern()s.
    void build();

    /// Search `text` for all pattern occurrences.
    /// Returns matches sorted by end_pos, then pattern_id.
    /// build() must have been called first.
    std::vector<Match> search(std::string_view text) const;

    /// Return the number of patterns added.
    std::size_t pattern_count() const noexcept { return patterns_.size(); }

    /// Return patterns in insertion order.
    const std::vector<std::string>& patterns() const noexcept { return patterns_; }

private:
    static constexpr int kAlpha = 256; // full byte alphabet

    struct Node {
        int next[kAlpha];
        int fail;       // failure (suffix) link
        int dict;       // dictionary link (nearest ancestor that is a pattern end)
        int output;     // pattern index if this node is end of a pattern, else -1
        Node() : fail(0), dict(-1), output(-1) {
            std::fill(std::begin(next), std::end(next), -1);
        }
    };

    std::vector<Node> trie_;
    std::vector<std::string> patterns_;
    bool built_ = false;

    int new_node();
};

} // namespace algorithms::strings::aho_corasick


