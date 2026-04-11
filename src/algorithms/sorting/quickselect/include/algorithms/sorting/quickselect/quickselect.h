#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace algorithms::sorting::quickselect {

// QuickSelect — find the k-th smallest element in O(n) expected time.
//
// After the call, the element at position `nth` is the element that would
// be there if the range were fully sorted. Elements before `nth` are ≤ *nth,
// and elements after are ≥ *nth (but not necessarily sorted).
//
// - Expected time:  O(n).
// - Worst-case time: O(n^2) (mitigated by median-of-three pivot).
// - Space: O(1) auxiliary (in-place).
//
// Requirements:
// - RandomIt must model RandomAccessIterator.
// - Compare must impose a strict-weak ordering.

template <class RandomIt, class Compare>
void quickselect(RandomIt first, RandomIt nth, RandomIt last, Compare comp);

template <class RandomIt>
void quickselect(RandomIt first, RandomIt nth, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    quickselect(first, nth, last, std::less<T>{});
}

namespace detail {

template <class RandomIt>
inline RandomIt median_of_three(RandomIt a, RandomIt b, RandomIt c) {
    if (*a < *b) {
        if (*b < *c) return b;       // a < b < c
        if (*a < *c) return c;       // a < c <= b
        return a;                     // c <= a < b
    }
    // *b <= *a
    if (*a < *c) return a;           // b <= a < c
    if (*b < *c) return c;           // b < c <= a
    return b;                         // c <= b <= a
}

// Partition [first, last) around pivot value.
// Returns pair (lt, gt) where:
//   [first, lt)  < pivot
//   [lt, gt)     == pivot
//   [gt, last)   > pivot
template <class RandomIt, class Compare>
std::pair<RandomIt, RandomIt> three_way_partition(RandomIt first, RandomIt last, Compare comp) {
    auto mid = first + (last - first) / 2;
    auto pivot_it = median_of_three(first, mid, std::prev(last));
    auto pivot = *pivot_it;

    RandomIt lt = first;
    RandomIt i  = first;
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
    return {lt, gt};
}

} // namespace detail

template <class RandomIt, class Compare>
void quickselect(RandomIt first, RandomIt nth, RandomIt last, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "quickselect requires RandomAccessIterator");

    while (last - first > 1) {
        auto [lt, gt] = detail::three_way_partition(first, last, comp);

        if (nth < lt) {
            last = lt;        // target is in the left partition
        } else if (nth >= gt) {
            first = gt;       // target is in the right partition
        } else {
            return;           // nth is in the equal-to-pivot region — done
        }
    }
}

} // namespace algorithms::sorting::quickselect

