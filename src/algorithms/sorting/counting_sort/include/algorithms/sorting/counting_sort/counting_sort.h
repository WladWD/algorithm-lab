#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

namespace algorithms::sorting::counting_sort {

// Stable counting sort for non-negative integers.
//
// - Time:  O(n + k) where k = max element value.
// - Space: O(n + k).
// - Stable: equal elements preserve their relative order.
//
// The input must contain non-negative integers (or be mapped to such).

/// Sort `data` in place using counting sort.
/// Elements must be non-negative.
void counting_sort(std::vector<int64_t>& data);

/// Sort `data` in place, given an explicit upper bound on values.
/// All elements must satisfy 0 <= data[i] <= max_val.
void counting_sort(std::vector<int64_t>& data, int64_t max_val);

} // namespace algorithms::sorting::counting_sort

