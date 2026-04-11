#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace algorithms::sorting::interpolation_search {

// Interpolation Search on a sorted random-access range of arithmetic values.
//
// Instead of always probing the midpoint (as binary search does), interpolation
// search estimates the position of the target by linear interpolation on the
// values at the current endpoints.  This yields O(log log n) expected time
// on uniformly distributed data, but degrades to O(n) in the worst case.
//
// Requirements:
// - RandomIt must model RandomAccessIterator.
// - The value type must support subtraction yielding a type convertible to double
//   (arithmetic types).
// - The range [first, last) must be sorted in ascending order.

namespace detail {

/// Core implementation: returns iterator to an element equal to `value`,
/// or `last` if not found.
template <class RandomIt, class T>
RandomIt interpolation_search_impl(RandomIt first, RandomIt last, const T& value) {
    auto lo = first;
    auto hi = last;
    if (lo == hi) return last;
    --hi; // hi now points to the last valid element

    while (lo <= hi && value >= *lo && value <= *hi) {
        if (lo == hi) {
            if (*lo == value) return lo;
            return last;
        }

        // Interpolation formula:
        //   pos = lo + ((value - *lo) * (hi - lo)) / (*hi - *lo)
        // Use double to avoid integer overflow on large ranges.
        auto range_val = static_cast<double>(*hi) - static_cast<double>(*lo);
        auto range_idx = static_cast<std::ptrdiff_t>(hi - lo);
        auto offset = static_cast<std::ptrdiff_t>(
            (static_cast<double>(value) - static_cast<double>(*lo))
            / range_val * static_cast<double>(range_idx)
        );

        // Clamp to valid range (guards against floating-point rounding).
        if (offset < 0) offset = 0;
        if (offset > range_idx) offset = range_idx;

        auto mid = lo + offset;

        if (*mid < value) {
            lo = mid + 1;
        } else if (*mid > value) {
            if (mid == first) return last; // prevent underflow
            hi = mid - 1;
        } else {
            return mid; // found
        }
    }

    return last; // not found
}

/// Lower-bound variant: returns iterator to the first element >= value.
/// Falls back to linear scan around the interpolated probe for correctness.
template <class RandomIt, class T>
RandomIt interpolation_lower_bound_impl(RandomIt first, RandomIt last, const T& value) {
    if (first == last) return last;

    auto lo = first;
    auto hi = last;

    while (lo < hi) {
        // If value is out of range, short-circuit.
        if (value <= *lo) return lo;
        if (value > *(hi - 1)) return hi;

        auto range_val = static_cast<double>(*(hi - 1)) - static_cast<double>(*lo);
        auto range_idx = static_cast<std::ptrdiff_t>(hi - lo);

        std::ptrdiff_t offset;
        if (range_val == 0.0) {
            offset = 0;
        } else {
            offset = static_cast<std::ptrdiff_t>(
                (static_cast<double>(value) - static_cast<double>(*lo))
                / range_val * static_cast<double>(range_idx - 1)
            );
        }

        if (offset < 0) offset = 0;
        if (offset >= range_idx) offset = range_idx - 1;

        auto mid = lo + offset;

        if (*mid < value) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return lo;
}

} // namespace detail

/// Search for `value` in the sorted range [first, last).
/// Returns iterator to an element equal to `value`, or `last` if not found.
///
/// Best for uniformly distributed numeric data.
/// Expected time: O(log log n).  Worst case: O(n).
template <class RandomIt, class T>
RandomIt interpolation_search(RandomIt first, RandomIt last, const T& value) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "interpolation_search requires RandomAccessIterator");
    static_assert(std::is_arithmetic_v<T>,
                  "interpolation_search requires an arithmetic value type");
    return detail::interpolation_search_impl(first, last, value);
}

/// Returns iterator to the first element >= value (lower_bound semantics),
/// using interpolation to estimate the probe position.
template <class RandomIt, class T>
RandomIt interpolation_lower_bound(RandomIt first, RandomIt last, const T& value) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "interpolation_lower_bound requires RandomAccessIterator");
    static_assert(std::is_arithmetic_v<T>,
                  "interpolation_lower_bound requires an arithmetic value type");
    return detail::interpolation_lower_bound_impl(first, last, value);
}

/// Returns true if `value` exists in the sorted range [first, last).
template <class RandomIt, class T>
bool interpolation_search_exists(RandomIt first, RandomIt last, const T& value) {
    auto it = interpolation_search(first, last, value);
    return it != last;
}

} // namespace algorithms::sorting::interpolation_search

