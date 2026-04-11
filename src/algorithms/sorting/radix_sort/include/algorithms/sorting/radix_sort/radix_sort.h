#pragma once

#include <cstdint>
#include <vector>

namespace algorithms::sorting::radix_sort {

// LSD Radix Sort for non-negative integers.
//
// - Time:  O(d * (n + b)) where d = number of digits, b = base (256).
// - Space: O(n + b).
// - Stable: yes (uses counting sort per digit).
//
// For 64-bit integers with base 256: d = 8, so time is O(8 * (n + 256)) ≈ O(n).

/// Sort `data` in ascending order using LSD radix sort.
/// Elements must be non-negative.
void radix_sort(std::vector<uint64_t>& data);

/// Sort `data` in ascending order, supporting negative values.
/// Uses offset to map all values to non-negative range.
void radix_sort_signed(std::vector<int64_t>& data);

} // namespace algorithms::sorting::radix_sort

