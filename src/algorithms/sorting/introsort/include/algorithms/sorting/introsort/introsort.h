#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace algorithms::sorting::introsort {

// IntroSort — hybrid sort: QuickSort + HeapSort + InsertionSort.
//
// Begins with QuickSort (median-of-three). If recursion depth exceeds
// 2*log2(n), switches to HeapSort on the current sub-range.
// Sub-ranges of ≤16 elements use insertion sort.
//
// - Time:  O(n log n) worst-case (guaranteed by HeapSort fallback).
// - Space: O(log n) stack.
// - Unstable.
//
// This is the strategy behind most std::sort implementations.

template <class RandomIt, class Compare>
void introsort(RandomIt first, RandomIt last, Compare comp);

template <class RandomIt>
void introsort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    introsort(first, last, std::less<T>{});
}

namespace detail {

template <class RandomIt, class Compare>
void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    for (auto it = first + 1; it < last; ++it) {
        auto key = std::move(*it);
        auto jt = it;
        while (jt > first) {
            auto prev = jt - 1;
            if (!comp(key, *prev)) break;
            *jt = std::move(*prev);
            jt = prev;
        }
        *jt = std::move(key);
    }
}

template <class RandomIt, class Compare>
void sift_down(RandomIt first, std::size_t n, std::size_t i, Compare comp) {
    while (true) {
        std::size_t largest = i;
        std::size_t left  = 2 * i + 1;
        std::size_t right = 2 * i + 2;
        if (left < n && comp(*(first + largest), *(first + left)))
            largest = left;
        if (right < n && comp(*(first + largest), *(first + right)))
            largest = right;
        if (largest == i) return;
        std::iter_swap(first + i, first + largest);
        i = largest;
    }
}

template <class RandomIt, class Compare>
void heapsort_range(RandomIt first, RandomIt last, Compare comp) {
    auto n = static_cast<std::size_t>(last - first);
    if (n <= 1) return;
    for (std::size_t i = n / 2; i-- > 0;)
        sift_down(first, n, i, comp);
    for (std::size_t i = n - 1; i > 0; --i) {
        std::iter_swap(first, first + i);
        sift_down(first, i, 0, comp);
    }
}

template <class RandomIt, class Compare>
void introsort_impl(RandomIt first, RandomIt last, Compare comp, int depth_limit) {
    while (last - first > 16) {
        if (depth_limit <= 0) {
            heapsort_range(first, last, comp);
            return;
        }
        --depth_limit;

        // Median-of-three pivot selection.
        auto mid = first + (last - first) / 2;
        auto back = last - 1;

        // Sort three candidates.
        if (comp(*mid, *first)) std::iter_swap(first, mid);
        if (comp(*back, *first)) std::iter_swap(first, back);
        if (comp(*back, *mid)) std::iter_swap(mid, back);

        // Pivot is at mid; move to second-to-last position.
        std::iter_swap(mid, back - 1);
        auto pivot_it = back - 1;
        auto pivot = *pivot_it;

        // Hoare partition around pivot.
        auto lo = first + 1;
        auto hi = back - 2;
        while (true) {
            while (comp(*lo, pivot)) ++lo;
            while (comp(pivot, *hi)) --hi;
            if (lo >= hi) break;
            std::iter_swap(lo, hi);
            ++lo;
            --hi;
        }
        // Place pivot in its final position.
        std::iter_swap(lo, pivot_it);

        // Recurse on the smaller side, loop on the larger.
        if (lo - first < last - lo - 1) {
            introsort_impl(first, lo, comp, depth_limit);
            first = lo + 1;
        } else {
            introsort_impl(lo + 1, last, comp, depth_limit);
            last = lo;
        }
    }
}

} // namespace detail

template <class RandomIt, class Compare>
void introsort(RandomIt first, RandomIt last, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "introsort requires RandomAccessIterator");

    const auto n = static_cast<std::size_t>(last - first);
    if (n <= 1) return;

    int depth_limit = static_cast<int>(2.0 * std::log2(static_cast<double>(n)));

    detail::introsort_impl(first, last, comp, depth_limit);

    // Final insertion sort pass to clean up the ≤16-element partitions.
    detail::insertion_sort(first, last, comp);
}

} // namespace algorithms::sorting::introsort

