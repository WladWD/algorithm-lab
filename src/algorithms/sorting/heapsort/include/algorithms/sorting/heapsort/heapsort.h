#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace algorithms::sorting::heapsort {

// In-place, unstable heap sort.
//
// - Time:  O(n log n) worst-case.
// - Space: O(1) auxiliary.
// - Unstable: equal elements may change relative order.
//
// Requirements:
// - RandomIt must model RandomAccessIterator.
// - Compare must impose a strict-weak ordering.

template <class RandomIt, class Compare>
void heapsort(RandomIt first, RandomIt last, Compare comp);

template <class RandomIt>
void heapsort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    heapsort(first, last, std::less<T>{});
}

namespace detail {

// Sift-down: restore the max-heap property rooted at index `i` within `[0, n)`.
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

} // namespace detail

template <class RandomIt, class Compare>
void heapsort(RandomIt first, RandomIt last, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "heapsort requires RandomAccessIterator");

    auto n = static_cast<std::size_t>(last - first);
    if (n <= 1) return;

    // Phase 1: build a max-heap (bottom-up).
    for (std::size_t i = n / 2; i-- > 0;) {
        detail::sift_down(first, n, i, comp);
    }

    // Phase 2: repeatedly extract the max and shrink the heap.
    for (std::size_t i = n - 1; i > 0; --i) {
        std::iter_swap(first, first + i);
        detail::sift_down(first, i, 0, comp);
    }
}

} // namespace algorithms::sorting::heapsort

