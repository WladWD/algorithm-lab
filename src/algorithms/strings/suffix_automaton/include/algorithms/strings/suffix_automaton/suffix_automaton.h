#pragma once

#include <array>
#include <cstddef>
#include <string_view>
#include <vector>

namespace algorithms::strings::suffix_automaton {

// Suffix Automaton (SAM) — compact representation of all substrings of a string.
//
// Built online in O(n) time and O(n) space (at most 2n states, 3n transitions).
// Each state represents an equivalence class of substrings (its endpos set).
// The suffix link of a state points to the state whose endpos set is the smallest
// proper superset — forming the suffix-link tree used for occurrence counting.
//
// Key properties:
//  - Every substring of s corresponds to a path from the initial state.
//  - count_occurrences runs in O(m) after O(n) preprocessing.
//  - count_distinct_substrings runs in O(n) after build.

class SuffixAutomaton {
public:
    explicit SuffixAutomaton(std::string_view s);

    /// True if pattern is a substring of the original string.  O(m).
    bool contains(std::string_view pattern) const;

    /// Number of times pattern occurs in the original string.  O(m).
    /// Returns 0 for empty or absent patterns.
    long long count_occurrences(std::string_view pattern) const;

    /// Total number of distinct non-empty substrings of the original string.  O(n).
    long long count_distinct_substrings() const;

    /// Number of SAM states (≤ 2n).
    std::size_t num_states() const noexcept { return st_.size(); }

private:
    struct State {
        int  len  = 0;
        int  link = -1;
        long long cnt  = 0; // occurrence count (populated after build)
        std::array<int, 256> next{};

        State() { next.fill(-1); }
    };

    std::vector<State> st_;
    int last_ = 0; // index of the state for the whole string

    void extend(unsigned char c);
    void compute_counts(); // propagate cnt via suffix-link tree
};

} // namespace algorithms::strings::suffix_automaton
