#include "algorithms/strings/kmp/kmp.h"

#include <algorithm>

namespace algorithms::strings::kmp {

std::vector<int> prefix_function(std::string_view s) {
    const int n = static_cast<int>(s.size());
    std::vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

std::vector<std::size_t> search(std::string_view text, std::string_view pattern) {
    std::vector<std::size_t> result;
    if (pattern.empty()) return result;

    const std::size_t n = text.size(), m = pattern.size();

    // Build prefix function on the concatenated string: pattern + '#' + text.
    // '#' is a sentinel that won't appear in either (value 0).
    // We process character by character without materialising the full string.
    std::vector<int> pi(m, 0);
    // Phase 1: build pi for pattern alone.
    for (std::size_t i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j]) j = pi[j - 1];
        if (pattern[i] == pattern[j]) ++j;
        pi[i] = j;
    }

    // Phase 2: slide over text, tracking the current match length j.
    int j = 0;
    for (std::size_t i = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j]) j = pi[j - 1];
        if (text[i] == pattern[j]) ++j;
        if (static_cast<std::size_t>(j) == m) {
            result.push_back(i + 1 - m);
            j = pi[m - 1];
        }
    }
    return result;
}

std::size_t count_occurrences(std::string_view text, std::string_view pattern) {
    return search(text, pattern).size();
}

std::vector<std::size_t> borders(std::string_view s) {
    auto pi = prefix_function(s);
    std::vector<std::size_t> result;
    int b = pi.empty() ? 0 : pi.back();
    while (b > 0) {
        result.push_back(static_cast<std::size_t>(b));
        b = pi[b - 1];
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::size_t period(std::string_view s) {
    if (s.empty()) return 0;
    auto pi = prefix_function(s);
    std::size_t n = s.size();
    std::size_t p = n - static_cast<std::size_t>(pi.back());
    // p is a period iff n % p == 0
    return (n % p == 0) ? p : n;
}

} // namespace algorithms::strings::kmp


