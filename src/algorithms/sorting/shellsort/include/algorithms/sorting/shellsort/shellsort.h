#pragma once

#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace algorithms::sorting::shellsort {

// ShellSort — in-place, unstable, diminishing-increment insertion sort.
//
// Uses the Ciura gap sequence (empirically optimal for general data):
//   1, 4, 10, 23, 57, 132, 301, 701, 1750, ...  (extended by *2.25)
//
// - Time:  O(n^(4/3)) empirical with Ciura gaps; O(n log² n) with good sequences.
// - Space: O(1) auxiliary.
// - Unstable.

template <class RandomIt, class Compare>
void shellsort(RandomIt first, RandomIt last, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "shellsort requires RandomAccessIterator");

    const auto n = static_cast<std::size_t>(last - first);
    if (n <= 1) return;

    // Ciura gap sequence, extended by factor 2.25.
    static constexpr std::size_t ciura[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    constexpr std::size_t ciura_len = sizeof(ciura) / sizeof(ciura[0]);

    // Build the gap sequence up to n.
    std::size_t gaps[64];
    int num_gaps = 0;

    for (std::size_t i = 0; i < ciura_len && ciura[i] < n; ++i) {
        gaps[num_gaps++] = ciura[i];
    }
    if (num_gaps > 0) {
        std::size_t g = gaps[num_gaps - 1];
        while (true) {
            g = static_cast<std::size_t>(g * 2.25);
            if (g >= n) break;
            gaps[num_gaps++] = g;
        }
    } else {
        gaps[num_gaps++] = 1;
    }

    // Process gaps from largest to smallest.
    for (int gi = num_gaps - 1; gi >= 0; --gi) {
        const std::size_t gap = gaps[gi];
        for (std::size_t i = gap; i < n; ++i) {
            auto key = std::move(*(first + i));
            std::size_t j = i;
            while (j >= gap && comp(key, *(first + j - gap))) {
                *(first + j) = std::move(*(first + j - gap));
                j -= gap;
            }
            *(first + j) = std::move(key);
        }
    }
}

template <class RandomIt>
void shellsort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    shellsort(first, last, std::less<T>{});
}

} // namespace algorithms::sorting::shellsort

