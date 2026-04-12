#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>

namespace algorithms::sorting::exponential_search {

// Exponential Search — find a value in a sorted range by first exponentially
// expanding to find a bounding range, then binary-searching within it.
//
// Particularly useful when:
// - The target is near the beginning of a large sorted array.
// - The array size is unknown or unbounded (e.g., infinite/streamed data).
//
// - Time:  O(log i) where i is the position of the target.
// - Space: O(1).

namespace detail {

template <class RandomIt, class T, class Compare>
RandomIt binary_search_range(RandomIt first, RandomIt last, const T& value,
                             Compare comp, RandomIt not_found) {
    while (first < last) {
        auto mid = first + (last - first) / 2;
        if (comp(*mid, value)) {
            first = mid + 1;
        } else if (comp(value, *mid)) {
            last = mid;
        } else {
            return mid; // found
        }
    }
    return not_found; // not found
}

template <class RandomIt, class T, class Compare>
RandomIt lower_bound_impl(RandomIt first, RandomIt last, const T& value, Compare comp) {
    while (first < last) {
        auto mid = first + (last - first) / 2;
        if (comp(*mid, value)) {
            first = mid + 1;
        } else {
            last = mid;
        }
    }
    return first;
}

} // namespace detail

/// Search for `value` in [first, last). Returns iterator to found element, or `last`.
/// Time: O(log i) where i is the index of the target.
template <class RandomIt, class T, class Compare>
RandomIt exponential_search(RandomIt first, RandomIt last, const T& value, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "exponential_search requires RandomAccessIterator");

    const auto n = last - first;
    if (n <= 0) return last;

    // Phase 1: exponential expansion to find bounds.
    std::ptrdiff_t bound = 1;
    while (bound < n && comp(*(first + bound), value)) {
        bound *= 2;
    }

    // Phase 2: binary search in [bound/2, min(bound+1, n)).
    auto lo = first + bound / 2;
    auto hi = first + std::min(bound + 1, n);
    return detail::binary_search_range(lo, hi, value, comp, last);
}

template <class RandomIt, class T>
RandomIt exponential_search(RandomIt first, RandomIt last, const T& value) {
    return exponential_search(first, last, value, std::less<T>{});
}

/// Lower-bound variant: returns first element >= value.
template <class RandomIt, class T, class Compare>
RandomIt exponential_lower_bound(RandomIt first, RandomIt last, const T& value, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "exponential_lower_bound requires RandomAccessIterator");

    const auto n = last - first;
    if (n <= 0) return last;

    std::ptrdiff_t bound = 1;
    while (bound < n && comp(*(first + bound), value)) {
        bound *= 2;
    }

    auto lo = first + bound / 2;
    auto hi = first + std::min(bound + 1, n);
    return detail::lower_bound_impl(lo, hi, value, comp);
}

template <class RandomIt, class T>
RandomIt exponential_lower_bound(RandomIt first, RandomIt last, const T& value) {
    return exponential_lower_bound(first, last, value, std::less<T>{});
}

/// Returns true if `value` exists in [first, last).
template <class RandomIt, class T>
bool exponential_search_exists(RandomIt first, RandomIt last, const T& value) {
    auto it = exponential_search(first, last, value);
    return it != last;
}

} // namespace algorithms::sorting::exponential_search



