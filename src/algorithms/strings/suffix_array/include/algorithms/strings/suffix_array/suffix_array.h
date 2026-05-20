#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace algorithms::strings::suffix_array {

// Suffix array with LCP array construction.
//
// Suffix array SA:  SA[i] = starting index of the i-th lexicographically smallest
//                   suffix of s.  Built in O(n log² n) via prefix-doubling.
//
// LCP array:  lcp[i] = length of the longest common prefix of s[SA[i-1]..] and
//             s[SA[i]..].  lcp[0] = 0 by convention.  Built in O(n) using
//             Kasai's algorithm.
//
// Pattern search uses two binary searches on the sorted suffix array: O(m log n).

/// Build the suffix array of s.
/// Returns a vector of length n where SA[i] is the start of the i-th smallest suffix.
/// Time: O(n log² n).  Space: O(n).
std::vector<int> build(std::string_view s);

/// Build the LCP array from string s and its precomputed suffix array sa.
/// lcp[i] = LCP(s[sa[i-1]..], s[sa[i]..]),  lcp[0] = 0.
/// Time: O(n) (Kasai's algorithm).  Space: O(n).
std::vector<int> build_lcp(std::string_view s, const std::vector<int>& sa);

/// Find all 0-indexed starting positions of pattern in text using the suffix array.
/// Returns a sorted vector of match positions.
/// Time: O(m log n).  Space: O(output).
std::vector<int> search(std::string_view text, std::string_view pattern,
                        const std::vector<int>& sa);

/// Count occurrences of pattern in text.  O(m log n).
int count_occurrences(std::string_view text, std::string_view pattern,
                      const std::vector<int>& sa);

} // namespace algorithms::strings::suffix_array
