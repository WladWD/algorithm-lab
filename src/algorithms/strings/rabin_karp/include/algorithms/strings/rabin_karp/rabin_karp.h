#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace algorithms::strings::rabin_karp {

// Rabin-Karp rolling-hash pattern matching.
//
// Computes a polynomial hash of the pattern, then slides a same-length window
// over the text, updating the hash in O(1) per step.  A hash match is always
// verified by direct string comparison, so results are always correct.
//
// Hash: h(s[i..i+m-1]) = s[i]*B^(m-1) + s[i+1]*B^(m-2) + ... + s[i+m-1]  (mod P)
// Roll: h_new = h_old * B  −  s[i] * B^m  +  s[i+m]                        (mod P)
//
// Single-pattern search: O(n + m) expected, O(n*m) worst case (all collisions).
// Multi-pattern (equal-length) search: O(n + k*m) expected, k = pattern count.

/// Find all 0-indexed starting positions of `pattern` in `text`.
/// Expected O(n + m). Worst-case O(n*m). Space: O(output).
std::vector<std::size_t> search(std::string_view text, std::string_view pattern);

/// Count occurrences of `pattern` in `text`. Expected O(n + m).
std::size_t count_occurrences(std::string_view text, std::string_view pattern);

/// Find all starting positions where any pattern from `patterns` occurs in `text`.
/// All patterns must have the same length; behaviour is undefined otherwise.
/// Expected O(n + k*m) where k = patterns.size(). Results are sorted ascending.
std::vector<std::size_t> search_multiple(std::string_view text,
                                         const std::vector<std::string_view>& patterns);

} // namespace algorithms::strings::rabin_karp
