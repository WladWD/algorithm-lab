#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace algorithms::strings::z_function {

// Z-function (Z-array) string algorithm.
//
// z[i] = length of the longest string starting at s[i] that is also a prefix of s.
// z[0] is conventionally 0 (undefined / equals the whole string).
//
// Computed in O(n) using the Z-box invariant.
// Directly applicable to exact pattern matching and many other string problems.

/// Compute the Z-array for string `s`.
/// z[i] = length of the longest prefix of s that matches s[i..].
/// z[0] = 0 by convention.
/// Time: O(n). Space: O(n).
std::vector<std::size_t> z_function(std::string_view s);

/// Find all 0-indexed starting positions of `pattern` in `text`
/// using the Z-algorithm.
/// Time: O(n + m). Space: O(m).
std::vector<std::size_t> search(std::string_view text, std::string_view pattern);

/// Count occurrences of `pattern` in `text`. O(n + m).
std::size_t count_occurrences(std::string_view text, std::string_view pattern);

/// Return the number of distinct substrings of `s` that are also a prefix of `s`.
/// Equivalently: the number of distinct z-values > 0. O(n).
std::size_t prefix_occurrences(std::string_view s);

} // namespace algorithms::strings::z_function

