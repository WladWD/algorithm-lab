#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace algorithms::sorting::bucket_sort {

// Bucket Sort for floating-point values in [0, 1) or general numeric ranges.
//
// Distributes elements into n buckets, sorts each bucket (via insertion sort),
// then concatenates.
//
// - Expected time: O(n) for uniformly distributed data.
// - Worst case: O(n²) (all elements in one bucket).
// - Space: O(n + k) where k = number of buckets.
// - Stable (within-bucket sort is stable insertion sort).

/// Sort a vector of doubles. Values should ideally be in [min_val, max_val).
/// `num_buckets` defaults to n if 0.
void bucket_sort(std::vector<double>& data, std::size_t num_buckets = 0);

/// Sort a vector of non-negative integers using bucket sort.
void bucket_sort_int(std::vector<int64_t>& data, std::size_t num_buckets = 0);

} // namespace algorithms::sorting::bucket_sort

