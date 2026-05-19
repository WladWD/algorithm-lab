#include "algorithms/strings/z_function/z_function.h"

#include <algorithm>
#include <unordered_set>

namespace algorithms::strings::z_function {

std::vector<std::size_t> z_function(std::string_view s) {
    const std::size_t n = s.size();
    std::vector<std::size_t> z(n, 0);
    if (n == 0) return z;

    std::size_t l = 0, r = 0; // current Z-box: s[l..r-1] == s[0..r-l-1]
    for (std::size_t i = 1; i < n; ++i) {
        if (i < r) {
            z[i] = std::min(r - i, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

std::vector<std::size_t> search(std::string_view text, std::string_view pattern) {
    std::vector<std::size_t> result;
    if (pattern.empty()) return result;

    const std::size_t m = pattern.size();
    const std::size_t n = text.size();
    if (m > n) return result;

    // Build Z-array of "pattern + sentinel + text".
    // Process character-by-character without allocating the full concatenation.
    // We compute the Z-function in two phases using the virtual string p#t.

    const std::size_t total = m + 1 + n;

    // Lambda to get char from virtual string.
    auto get = [&](std::size_t i) -> char {
        if (i < m)        return pattern[i];
        if (i == m)       return '\0'; // sentinel (no match possible)
        return text[i - m - 1];
    };

    std::size_t l = 0, r = 0;
    std::vector<std::size_t> z(total, 0);

    for (std::size_t i = 1; i < total; ++i) {
        if (i < r) {
            z[i] = std::min(r - i, z[i - l]);
        }
        while (i + z[i] < total && get(z[i]) == get(i + z[i])) {
            ++z[i];
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
        // Positions in text start at offset m+1
        if (i > m && z[i] == m) {
            result.push_back(i - m - 1);
        }
    }
    return result;
}

std::size_t count_occurrences(std::string_view text, std::string_view pattern) {
    return search(text, pattern).size();
}

std::size_t prefix_occurrences(std::string_view s) {
    auto z = z_function(s);
    std::unordered_set<std::size_t> distinct;
    for (std::size_t i = 1; i < z.size(); ++i) {
        if (z[i] > 0) distinct.insert(z[i]);
    }
    return distinct.size();
}

} // namespace algorithms::strings::z_function

