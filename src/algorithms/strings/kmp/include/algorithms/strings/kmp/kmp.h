#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace algorithms::strings::kmp {

// Knuth-Morris-Pratt (KMP) string matching.
//
// Builds the prefix function (failure function) in O(n), then uses it to
// search for a pattern in a text in O(n + m) total time, O(1) extra space
// per character (O(m) for the prefix table).
//
// prefix_function[i] = length of the longest proper prefix of s[0..i]
//                      that is also a suffix of s[0..i].

/// Compute the prefix function (failure function) for string `s`.
/// Result[i] = length of the longest proper prefix of s[0..i] that is also a suffix.
/// Time: O(n). Space: O(n).
std::vector<int> prefix_function(std::string_view s);

/// Find all 0-indexed starting positions of `pattern` in `text`.
/// Time: O(n + m). Space: O(m).
std::vector<std::size_t> search(std::string_view text, std::string_view pattern);

/// Count the number of occurrences of `pattern` in `text`.
/// Time: O(n + m). Space: O(m).
std::size_t count_occurrences(std::string_view text, std::string_view pattern);

/// Return all distinct borders (proper prefixes that are also suffixes) of `s`,
/// sorted by increasing length.
std::vector<std::size_t> borders(std::string_view s);

/// Return the period of string `s`: the smallest p such that s[i] == s[i % p]
/// for all i. Returns s.size() if the string has no period < its length.
std::size_t period(std::string_view s);

} // namespace algorithms::strings::kmp

