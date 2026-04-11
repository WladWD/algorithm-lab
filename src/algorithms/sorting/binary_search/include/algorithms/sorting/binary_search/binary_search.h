#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace algorithms::sorting::binary_search {

namespace detail {

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

template <class RandomIt, class T, class Compare>
RandomIt upper_bound_impl(RandomIt first, RandomIt last, const T& value, Compare comp) {
    while (first < last) {
        auto mid = first + (last - first) / 2;
        if (comp(value, *mid)) {
            last = mid;
        } else {
            first = mid + 1;
        }
    }
    return first;
}

} // namespace detail

// Binary search variants on sorted random-access ranges.
//
// All functions require the range [first, last) to be sorted w.r.t. `comp`.
//
// lower_bound: first position where `value` could be inserted (≥ value).
// upper_bound: last  position where `value` could be inserted (> value).
// binary_search: true if `value` exists in the range.
// equal_range: pair of (lower_bound, upper_bound).

/// Returns iterator to the first element that is >= value.
template <class RandomIt, class T, class Compare>
RandomIt lower_bound(RandomIt first, RandomIt last, const T& value, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "binary_search requires RandomAccessIterator");
    return detail::lower_bound_impl(first, last, value, comp);
}

template <class RandomIt, class T>
RandomIt lower_bound(RandomIt first, RandomIt last, const T& value) {
    return detail::lower_bound_impl(first, last, value, std::less<T>{});
}

/// Returns iterator to the first element that is > value.
template <class RandomIt, class T, class Compare>
RandomIt upper_bound(RandomIt first, RandomIt last, const T& value, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "binary_search requires RandomAccessIterator");
    return detail::upper_bound_impl(first, last, value, comp);
}

template <class RandomIt, class T>
RandomIt upper_bound(RandomIt first, RandomIt last, const T& value) {
    return detail::upper_bound_impl(first, last, value, std::less<T>{});
}

/// Returns true if `value` is present in [first, last).
template <class RandomIt, class T, class Compare>
bool binary_search(RandomIt first, RandomIt last, const T& value, Compare comp) {
    auto it = detail::lower_bound_impl(first, last, value, comp);
    return it != last && !comp(value, *it);
}

template <class RandomIt, class T>
bool binary_search(RandomIt first, RandomIt last, const T& value) {
    std::less<T> comp{};
    auto it = detail::lower_bound_impl(first, last, value, comp);
    return it != last && !comp(value, *it);
}

/// Returns {lower_bound, upper_bound} pair.
template <class RandomIt, class T, class Compare>
std::pair<RandomIt, RandomIt> equal_range(RandomIt first, RandomIt last,
                                           const T& value, Compare comp) {
    return {detail::lower_bound_impl(first, last, value, comp),
            detail::upper_bound_impl(first, last, value, comp)};
}

template <class RandomIt, class T>
std::pair<RandomIt, RandomIt> equal_range(RandomIt first, RandomIt last, const T& value) {
    return {detail::lower_bound_impl(first, last, value, std::less<T>{}),
            detail::upper_bound_impl(first, last, value, std::less<T>{})};
}

} // namespace algorithms::sorting::binary_search


