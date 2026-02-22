#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace algorithms::sorting::quicksort {

// QuickSort (three-way partition, median-of-three pivot) with a recursion depth guard.
//
// Notes:
// - This is an *unstable* sort.
// - Average time: O(n log n); worst-case: O(n^2).
// - To make worst-case behavior robust, this implementation falls back to std::sort
//   once recursion depth exceeds ~2*log2(n) (introsort-style guard).
//
// Requirements:
// - RandomIt must model RandomAccessIterator.
// - Compare must impose a strict-weak ordering.

template <class RandomIt, class Compare>
void quicksort(RandomIt first, RandomIt last, Compare comp);

template <class RandomIt> void quicksort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    quicksort(first, last, std::less<T>{});
}

namespace detail {

inline int depth_limit_for_size(std::size_t n) {
    if (n <= 1) {
        return 0;
    }
    const double lg = std::log2(static_cast<double>(n));
    return static_cast<int>(2.0 * lg) + 1;
}

template <class RandomIt, class Compare>
inline void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    for (auto it = first; it != last; ++it) {
        auto key = std::move(*it);
        auto jt = it;
        while (jt != first) {
            auto prev = std::prev(jt);
            if (!comp(key, *prev)) {
                break;
            }
            *jt = std::move(*prev);
            jt = prev;
        }
        *jt = std::move(key);
    }
}

template <class RandomIt>
inline typename std::iterator_traits<RandomIt>::value_type median_of_three(RandomIt a, RandomIt b,
                                                                           RandomIt c) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    const T& x = *a;
    const T& y = *b;
    const T& z = *c;

    // Return by value to avoid reference issues when elements move.
    if ((x < y && y < z) || (z < y && y < x)) {
        return y;
    }
    if ((y < x && x < z) || (z < x && x < y)) {
        return x;
    }
    return z;
}

template <class RandomIt, class Compare>
inline void quicksort_impl(RandomIt first, RandomIt last, Compare comp, int depth_limit) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "quicksort requires RandomAccessIterator");

    while (true) {
        const auto n = static_cast<std::size_t>(last - first);
        if (n <= 16) {
            insertion_sort(first, last, comp);
            return;
        }

        if (depth_limit <= 0) {
            std::sort(first, last, comp);
            return;
        }

        auto mid = first + (last - first) / 2;
        auto pivot = median_of_three(first, mid, std::prev(last));

        // 3-way partition: [< pivot] [== pivot] [> pivot]
        RandomIt lt = first;
        RandomIt i = first;
        RandomIt gt = last;

        while (i < gt) {
            if (comp(*i, pivot)) {
                std::iter_swap(lt, i);
                ++lt;
                ++i;
            } else if (comp(pivot, *i)) {
                --gt;
                std::iter_swap(i, gt);
            } else {
                ++i;
            }
        }

        const auto left_n = static_cast<std::size_t>(lt - first);
        const auto right_n = static_cast<std::size_t>(last - gt);

        --depth_limit;

        // Recurse on smaller side first; loop on the other side (tail recursion elimination).
        if (left_n < right_n) {
            quicksort_impl(first, lt, comp, depth_limit);
            first = gt;
        } else {
            quicksort_impl(gt, last, comp, depth_limit);
            last = lt;
        }
    }
}

} // namespace detail

template <class RandomIt, class Compare>
void quicksort(RandomIt first, RandomIt last, Compare comp) {
    const auto n = static_cast<std::size_t>(last - first);
    detail::quicksort_impl(first, last, comp, detail::depth_limit_for_size(n));
}

} // namespace algorithms::sorting::quicksort
