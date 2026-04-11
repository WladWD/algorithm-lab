#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <vector>

namespace algorithms::sorting::mergesort {

// Stable, comparison-based merge sort.
//
// - Time:  O(n log n) worst-case and average.
// - Space: O(n) auxiliary.
// - Stable: equal elements preserve their relative order.
//
// Requirements:
// - RandomIt must model RandomAccessIterator.
// - Compare must impose a strict-weak ordering.

template <class RandomIt, class Compare>
void mergesort(RandomIt first, RandomIt last, Compare comp);

template <class RandomIt>
void mergesort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    mergesort(first, last, std::less<T>{});
}

namespace detail {

template <class RandomIt, class Compare>
void merge(RandomIt first, RandomIt mid, RandomIt last, Compare comp,
           std::vector<typename std::iterator_traits<RandomIt>::value_type>& buf) {
    buf.clear();
    auto left = first;
    auto right = mid;

    while (left != mid && right != last) {
        // Use !comp(right, left) for stability (prefer left on ties).
        if (!comp(*right, *left)) {
            buf.push_back(std::move(*left));
            ++left;
        } else {
            buf.push_back(std::move(*right));
            ++right;
        }
    }
    while (left != mid) {
        buf.push_back(std::move(*left));
        ++left;
    }
    // Right-side remainder is already in place — copy merged portion back.
    std::move(buf.begin(), buf.end(), first);
}

template <class RandomIt, class Compare>
void mergesort_impl(RandomIt first, RandomIt last, Compare comp,
                    std::vector<typename std::iterator_traits<RandomIt>::value_type>& buf) {
    const auto n = last - first;
    if (n <= 1) return;

    // Insertion sort for small ranges.
    if (n <= 16) {
        for (auto it = first + 1; it != last; ++it) {
            auto key = std::move(*it);
            auto jt = it;
            while (jt != first) {
                auto prev = std::prev(jt);
                if (!comp(key, *prev)) break;
                *jt = std::move(*prev);
                jt = prev;
            }
            *jt = std::move(key);
        }
        return;
    }

    auto mid = first + n / 2;
    mergesort_impl(first, mid, comp, buf);
    mergesort_impl(mid, last, comp, buf);

    // Skip merge if already sorted.
    if (!comp(*mid, *std::prev(mid))) return;

    merge(first, mid, last, comp, buf);
}

} // namespace detail

template <class RandomIt, class Compare>
void mergesort(RandomIt first, RandomIt last, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "mergesort requires RandomAccessIterator");
    const auto n = static_cast<std::size_t>(last - first);
    using T = typename std::iterator_traits<RandomIt>::value_type;
    std::vector<T> buf;
    buf.reserve(n);
    detail::mergesort_impl(first, last, comp, buf);
}

} // namespace algorithms::sorting::mergesort

