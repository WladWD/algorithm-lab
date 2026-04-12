#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace algorithms::sorting::median_of_medians {

// Median of Medians (BFPRT) — deterministic O(n) selection algorithm.
//
// Finds the k-th smallest element in worst-case O(n) time by choosing
// a guaranteed-good pivot (the median of medians of groups of 5).
//
// After the call, the element at `nth` is the element that would be there
// if the range were fully sorted. Elements before `nth` are ≤ *nth,
// elements after are ≥ *nth.
//
// - Time:  O(n) worst-case (guaranteed).
// - Space: O(log n) stack.
// - Unstable.

template <class RandomIt, class Compare>
void select(RandomIt first, RandomIt nth, RandomIt last, Compare comp);

template <class RandomIt>
void select(RandomIt first, RandomIt nth, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    select(first, nth, last, std::less<T>{});
}

namespace detail {

template <class RandomIt, class Compare>
void insertion_sort_range(RandomIt first, RandomIt last, Compare comp) {
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

// Partition [first, last) around the element at pivot_it.
// Returns iterator to the pivot's final position.
template <class RandomIt, class Compare>
RandomIt partition_around(RandomIt first, RandomIt last, RandomIt pivot_it, Compare comp) {
    auto pivot_val = *pivot_it;
    // Move pivot to end.
    std::iter_swap(pivot_it, last - 1);

    RandomIt store = first;
    for (auto it = first; it < last - 1; ++it) {
        if (comp(*it, pivot_val)) {
            std::iter_swap(it, store);
            ++store;
        }
    }
    std::iter_swap(store, last - 1);
    return store;
}

template <class RandomIt, class Compare>
void mom_select(RandomIt first, RandomIt nth, RandomIt last, Compare comp) {
    while (true) {
        auto n = last - first;
        if (n <= 5) {
            insertion_sort_range(first, last, comp);
            return;
        }

        // Step 1: Divide into groups of 5, find median of each group.
        auto num_groups = (n + 4) / 5;
        for (decltype(num_groups) i = 0; i < num_groups; ++i) {
            auto group_start = first + i * 5;
            auto group_end = group_start + 5;
            if (group_end > last) group_end = last;
            insertion_sort_range(group_start, group_end, comp);

            // Move the median of this group to position first + i.
            auto group_mid = group_start + (group_end - group_start) / 2;
            std::iter_swap(first + i, group_mid);
        }

        // Step 2: Recursively find the median of the medians.
        auto medians_end = first + num_groups;
        auto medians_mid = first + num_groups / 2;
        mom_select(first, medians_mid, medians_end, comp);

        // Step 3: Partition the full range around the median-of-medians.
        auto pivot_pos = partition_around(first, last, medians_mid, comp);

        // Step 4: Recurse on the appropriate side.
        if (nth == pivot_pos) {
            return;
        } else if (nth < pivot_pos) {
            last = pivot_pos;
        } else {
            first = pivot_pos + 1;
        }
    }
}

} // namespace detail

template <class RandomIt, class Compare>
void select(RandomIt first, RandomIt nth, RandomIt last, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "median_of_medians::select requires RandomAccessIterator");

    if (last - first <= 1) return;
    if (nth < first || nth >= last) return;

    detail::mom_select(first, nth, last, comp);
}

} // namespace algorithms::sorting::median_of_medians

