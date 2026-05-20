#include "algorithms/strings/suffix_tree/suffix_tree.h"

#include <algorithm>
#include <limits>

namespace algorithms::strings::suffix_tree {

// ──────────────────────────── construction ────────────────────────────────

SuffixTree::SuffixTree(std::string_view s) {
    s_.reserve(s.size() + 1);
    s_.assign(s);
    s_ += '\1'; // unique sentinel: forces every suffix to end at a distinct leaf

    int n = static_cast<int>(s_.size());
    nd_.reserve(2 * n + 2);
    new_node(-1, -1); // node 0 = root (no incoming edge)

    for (int i = 0; i < n; ++i) step(i);

    mark(0, 0); // label leaf nodes with their suffix start indices
}

int SuffixTree::new_node(int start, int end) {
    nd_.emplace_back(start, end);
    return static_cast<int>(nd_.size()) - 1;
}

void SuffixTree::step(int i) {
    // Extend the tree by one character s_[i].
    // 'remaining_' tracks how many suffixes still need to be inserted.
    int size = i + 1;
    ++remaining_;
    last_new_ = -1;

    while (remaining_ > 0) {
        // Determine the first character of the active edge.
        if (active_len_ == 0) active_edge_ = i;
        auto aec = static_cast<unsigned char>(s_[active_edge_]);

        if (nd_[active_node_].ch[aec] == -1) {
            // Rule 2a: no edge — create a new open leaf.
            nd_[active_node_].ch[aec] = new_node(i, INF);
            if (last_new_ >= 0) {
                nd_[last_new_].link = active_node_;
                last_new_ = -1;
            }
        } else {
            int nxt = nd_[active_node_].ch[aec];
            int el  = nd_[nxt].len(size);

            if (active_len_ >= el) {
                // Walk down: the active point is past the end of this edge.
                active_edge_ += el;
                active_len_  -= el;
                active_node_  = nxt;
                continue; // re-examine with updated active point
            }

            // Active point is inside edge nxt.
            if (s_[nd_[nxt].start + active_len_] == s_[i]) {
                // Rule 3: character already in the tree — just extend active length.
                ++active_len_;
                if (last_new_ >= 0) nd_[last_new_].link = active_node_;
                break; // all remaining suffixes are implicitly present
            }

            // Rule 2b: mismatch inside an edge — split and insert.
            int split = new_node(nd_[nxt].start,
                                 nd_[nxt].start + active_len_ - 1); // edge up to split point
            nd_[active_node_].ch[aec] = split;

            nd_[nxt].start += active_len_; // nxt's edge now begins after split
            nd_[split].ch[static_cast<unsigned char>(s_[nd_[nxt].start])] = nxt;
            nd_[split].ch[static_cast<unsigned char>(s_[i])] = new_node(i, INF);

            if (last_new_ >= 0) nd_[last_new_].link = split;
            last_new_ = split;
        }

        // One more suffix has been inserted.
        --remaining_;

        // Update active point for the next suffix.
        if (active_node_ == 0 && active_len_ > 0) {
            --active_len_;
            active_edge_ = i - remaining_ + 1;
        } else if (nd_[active_node_].link > 0) {
            active_node_ = nd_[active_node_].link;
        } else {
            active_node_ = 0;
        }
    }
}

void SuffixTree::mark(int u, int depth) {
    bool leaf = true;
    for (int v : nd_[u].ch) {
        if (v == -1) continue;
        leaf = false;
        mark(v, depth + nd_[v].len(static_cast<int>(s_.size())));
    }
    if (leaf) nd_[u].idx = static_cast<int>(s_.size()) - depth;
}

// ──────────────────────────── queries ─────────────────────────────────────

// Returns the index of the deepest node whose path from root spells out pattern,
// or -1 if pattern is not found.  The node may be internal or a leaf.
int SuffixTree::locate(std::string_view pattern) const {
    int cur = 0, i = 0;
    int m = static_cast<int>(pattern.size());
    int sz = static_cast<int>(s_.size());
    while (i < m) {
        auto c = static_cast<unsigned char>(pattern[i]);
        int nxt = nd_[cur].ch[c];
        if (nxt == -1) return -1;
        int el = nd_[nxt].len(sz);
        for (int j = 0; j < el && i < m; ++j, ++i) {
            if (s_[nd_[nxt].start + j] != pattern[i]) return -1;
        }
        cur = nxt;
    }
    return cur;
}

bool SuffixTree::contains(std::string_view pattern) const {
    if (pattern.empty()) return !s_.empty();
    return locate(pattern) != -1;
}

void SuffixTree::collect(int u, std::vector<int>& out) const {
    bool leaf = true;
    for (int v : nd_[u].ch) {
        if (v == -1) continue;
        leaf = false;
        collect(v, out);
    }
    if (leaf && nd_[u].idx >= 0) out.push_back(nd_[u].idx);
}

std::vector<int> SuffixTree::search(std::string_view pattern) const {
    if (pattern.empty()) return {};
    int node = locate(pattern);
    if (node == -1) return {};
    std::vector<int> result;
    collect(node, result);
    std::sort(result.begin(), result.end());
    return result;
}

int SuffixTree::count_occurrences(std::string_view pattern) const {
    return static_cast<int>(search(pattern).size());
}

} // namespace algorithms::strings::suffix_tree
