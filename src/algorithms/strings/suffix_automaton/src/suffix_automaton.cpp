#include "algorithms/strings/suffix_automaton/suffix_automaton.h"

#include <algorithm>
#include <numeric>

namespace algorithms::strings::suffix_automaton {

SuffixAutomaton::SuffixAutomaton(std::string_view s) {
    // Each character adds at most 2 states.
    st_.reserve(2 * s.size() + 2);
    st_.emplace_back(); // state 0 = initial (root)
    st_[0].len  = 0;
    st_[0].link = -1;

    for (unsigned char c : s) extend(c);
    compute_counts();
}

void SuffixAutomaton::extend(unsigned char c) {
    // cur: new state representing the suffix ending at the new character.
    int cur = static_cast<int>(st_.size());
    st_.emplace_back();
    st_[cur].len = st_[last_].len + 1;
    st_[cur].cnt = 1; // this suffix appears once

    int p = last_;
    while (p != -1 && st_[p].next[c] == -1) {
        st_[p].next[c] = cur;
        p = st_[p].link;
    }

    if (p == -1) {
        st_[cur].link = 0;
    } else {
        int q = st_[p].next[c];
        if (st_[p].len + 1 == st_[q].len) {
            st_[cur].link = q; // q is already the correct suffix link
        } else {
            // Clone q to split the transition.
            int clone = static_cast<int>(st_.size());
            st_.push_back(st_[q]);          // copy transitions and link from q
            st_[clone].len = st_[p].len + 1;
            st_[clone].cnt = 0;             // clone does not introduce new occurrences

            while (p != -1 && st_[p].next[c] == q) {
                st_[p].next[c] = clone;
                p = st_[p].link;
            }
            st_[q].link    = clone;
            st_[cur].link  = clone;
        }
    }
    last_ = cur;
}

void SuffixAutomaton::compute_counts() {
    // Topological order of the suffix-link tree = order by decreasing len.
    int sz = static_cast<int>(st_.size());
    std::vector<int> order(sz);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [this](int a, int b) {
        return st_[a].len > st_[b].len;
    });
    for (int v : order) {
        if (st_[v].link != -1)
            st_[st_[v].link].cnt += st_[v].cnt;
    }
}

bool SuffixAutomaton::contains(std::string_view pattern) const {
    int cur = 0;
    for (unsigned char c : pattern) {
        int nxt = st_[cur].next[c];
        if (nxt == -1) return false;
        cur = nxt;
    }
    return true;
}

long long SuffixAutomaton::count_occurrences(std::string_view pattern) const {
    if (pattern.empty()) return 0;
    int cur = 0;
    for (unsigned char c : pattern) {
        int nxt = st_[cur].next[c];
        if (nxt == -1) return 0;
        cur = nxt;
    }
    return st_[cur].cnt;
}

long long SuffixAutomaton::count_distinct_substrings() const {
    long long total = 0;
    for (int i = 1; i < static_cast<int>(st_.size()); ++i)
        total += st_[i].len - st_[st_[i].link].len;
    return total;
}

} // namespace algorithms::strings::suffix_automaton
