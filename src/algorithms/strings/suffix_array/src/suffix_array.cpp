#include "algorithms/strings/suffix_array/suffix_array.h"

#include <algorithm>
#include <numeric>

namespace algorithms::strings::suffix_array {

std::vector<int> build(std::string_view s) {
    int n = static_cast<int>(s.size());
    if (n == 0) return {};

    std::vector<int> sa(n), rank_(n), tmp(n);
    std::iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; ++i) rank_[i] = static_cast<unsigned char>(s[i]);

    for (int k = 1; k < n; k <<= 1) {
        auto rank2 = [&](int i) -> int {
            return (i + k < n) ? rank_[i + k] : -1;
        };

        std::sort(sa.begin(), sa.end(), [&](int a, int b) {
            if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
            return rank2(a) < rank2(b);
        });

        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            bool diff = rank_[sa[i]] != rank_[sa[i - 1]] ||
                        rank2(sa[i]) != rank2(sa[i - 1]);
            tmp[sa[i]] = tmp[sa[i - 1]] + (diff ? 1 : 0);
        }
        rank_ = tmp;
        if (rank_[sa[n - 1]] == n - 1) break; // all ranks distinct
    }
    return sa;
}

std::vector<int> build_lcp(std::string_view s, const std::vector<int>& sa) {
    int n = static_cast<int>(s.size());
    std::vector<int> rank_(n), lcp(n, 0);
    for (int i = 0; i < n; ++i) rank_[sa[i]] = i;

    int h = 0;
    for (int i = 0; i < n; ++i) {
        if (rank_[i] == 0) { h = 0; continue; }
        int j = sa[rank_[i] - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h]) ++h;
        lcp[rank_[i]] = h;
        if (h > 0) --h;
    }
    return lcp;
}

std::vector<int> search(std::string_view text, std::string_view pattern,
                        const std::vector<int>& sa) {
    if (pattern.empty() || sa.empty()) return {};
    int n = static_cast<int>(sa.size());
    int m = static_cast<int>(pattern.size());

    // Binary search for the half-open range [lo, hi) in sa where
    // text[sa[i]..sa[i]+m-1] == pattern.
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (text.compare(sa[mid], m, pattern) < 0) lo = mid + 1;
        else hi = mid;
    }
    int left = lo;

    hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (text.compare(sa[mid], m, pattern) <= 0) lo = mid + 1;
        else hi = mid;
    }
    int right = lo;

    std::vector<int> result(sa.begin() + left, sa.begin() + right);
    std::sort(result.begin(), result.end());
    return result;
}

int count_occurrences(std::string_view text, std::string_view pattern,
                      const std::vector<int>& sa) {
    return static_cast<int>(search(text, pattern, sa).size());
}

} // namespace algorithms::strings::suffix_array
