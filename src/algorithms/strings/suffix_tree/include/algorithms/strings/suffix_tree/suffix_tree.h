#pragma once

#include <array>
#include <cstddef>
#include <string_view>
#include <vector>

namespace algorithms::strings::suffix_tree {

// Suffix Tree built with Ukkonen's online algorithm.
//
// Processes each character in amortised O(1), giving O(n) total build time and space.
// A unique sentinel character ('\1') is appended to the input to make the implicit
// suffix tree explicit (every suffix ends at a distinct leaf).
//
// Active-point representation maintains (active_node, active_edge, active_length)
// across phases so that each character triggers at most O(1) amortised node creations.
//
// Applications exposed:
//  - contains:          O(m)
//  - search (all hits): O(m + z) where z = number of occurrences
//  - count_occurrences: O(m + z)

class SuffixTree {
public:
    /// Build the suffix tree for s.  O(n) time and space.
    explicit SuffixTree(std::string_view s);

    /// True if pattern is a substring of s.  O(m).
    bool contains(std::string_view pattern) const;

    /// All 0-indexed starting positions of pattern in s, sorted ascending.  O(m + z).
    std::vector<int> search(std::string_view pattern) const;

    /// Number of occurrences of pattern in s.  O(m + z).
    int count_occurrences(std::string_view pattern) const;

    /// Number of nodes in the suffix tree (≤ 2n + 1).
    std::size_t num_nodes() const noexcept { return nd_.size(); }

private:
    static constexpr int INF = 0x7fff'ffff;

    struct Node {
        int start;   // edge label = s_[start .. min(end, size_-1)]  (inclusive)
        int end;     // INF for open leaves
        int link;    // suffix link (0 = root)
        int idx;     // suffix start index for leaves; -1 for internal nodes
        std::array<int, 256> ch;

        Node(int s, int e) : start(s), end(e), link(0), idx(-1) { ch.fill(-1); }

        int len(int cur_size) const noexcept {
            return std::min(end, cur_size - 1) - start + 1;
        }
    };

    std::string     s_;       // text + sentinel
    std::vector<Node> nd_;    // node pool; 0 = root

    // Ukkonen active-point state
    int active_node_  = 0;
    int active_edge_  = 0;   // index into s_ for the first char of active edge
    int active_len_   = 0;
    int remaining_    = 0;
    int last_new_     = -1;  // last newly created internal node awaiting suffix link

    int  new_node(int start, int end);
    void step(int i);                               // process s_[i]
    void mark(int node, int depth);                 // DFS to set leaf suffix indices
    void collect(int node, std::vector<int>& out) const; // DFS to gather leaf indices
    int  locate(std::string_view pattern) const;    // find node where pattern ends, or -1
};

} // namespace algorithms::strings::suffix_tree
