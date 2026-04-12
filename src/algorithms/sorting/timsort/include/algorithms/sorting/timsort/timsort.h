#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace algorithms::sorting::timsort {

// TimSort — stable, adaptive, O(n log n) comparison sort.
//
// Inspired by Python's built-in sort. Exploits existing runs (ascending or
// descending) in the data, merging them with a merge stack that guarantees
// balanced merges.
//
// - Time:  O(n log n) worst-case; O(n) on nearly-sorted data.
// - Space: O(n) auxiliary.
// - Stable.

template <class RandomIt, class Compare>
void timsort(RandomIt first, RandomIt last, Compare comp);

template <class RandomIt>
void timsort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    timsort(first, last, std::less<T>{});
}

namespace detail {

constexpr std::size_t kMinRun = 32;

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

// Compute minimum run length: if n < 32, return n.
// Otherwise, take the 6 most significant bits of n, adding 1 if any
// remaining bits are set (to ensure merges are balanced).
inline std::size_t compute_min_run(std::size_t n) {
    std::size_t r = 0;
    while (n >= kMinRun) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

// Find the length of the natural run starting at `first`.
// If descending, reverse it in-place to make it ascending.
template <class RandomIt, class Compare>
std::size_t find_run(RandomIt first, RandomIt last, Compare comp) {
    if (last - first <= 1) return static_cast<std::size_t>(last - first);

    auto it = first + 1;
    if (comp(*it, *first)) {
        // Strictly descending run.
        while (it < last - 1 && comp(*(it + 1), *it)) ++it;
        ++it;
        std::reverse(first, it);
    } else {
        // Non-descending run.
        while (it < last - 1 && !comp(*(it + 1), *it)) ++it;
        ++it;
    }
    return static_cast<std::size_t>(it - first);
}

template <class RandomIt, class Compare>
void merge_runs(RandomIt first, RandomIt mid, RandomIt last, Compare comp,
                std::vector<typename std::iterator_traits<RandomIt>::value_type>& buf) {
    buf.clear();
    // Copy the smaller side into the buffer for efficiency.
    auto left_len = mid - first;
    auto right_len = last - mid;

    if (left_len <= right_len) {
        buf.assign(std::make_move_iterator(first), std::make_move_iterator(mid));
        auto bi = buf.begin();
        auto be = buf.end();
        auto ri = mid;
        auto out = first;

        while (bi < be && ri < last) {
            if (!comp(*ri, *bi)) {  // stable: prefer left on ties
                *out++ = std::move(*bi++);
            } else {
                *out++ = std::move(*ri++);
            }
        }
        while (bi < be) *out++ = std::move(*bi++);
        // Right remainder is already in place.
    } else {
        buf.assign(std::make_move_iterator(mid), std::make_move_iterator(last));
        auto bi = buf.end();
        auto bs = buf.begin();
        auto li = mid;
        auto out = last;

        while (bi > bs && li > first) {
            if (comp(*(bi - 1), *(li - 1))) {
                *--out = std::move(*--li);
            } else {
                *--out = std::move(*--bi);
            }
        }
        while (bi > bs) *--out = std::move(*--bi);
    }
}

struct Run {
    std::size_t start;
    std::size_t length;
};

} // namespace detail

template <class RandomIt, class Compare>
void timsort(RandomIt first, RandomIt last, Compare comp) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "timsort requires RandomAccessIterator");

    const auto n = static_cast<std::size_t>(last - first);
    if (n <= 1) return;

    if (n <= detail::kMinRun) {
        detail::insertion_sort_range(first, last, comp);
        return;
    }

    using T = typename std::iterator_traits<RandomIt>::value_type;
    std::vector<T> buf;
    buf.reserve(n / 2 + 1);

    const std::size_t min_run = detail::compute_min_run(n);

    // Stack of pending runs to merge.
    std::vector<detail::Run> stack;
    stack.reserve(64);

    std::size_t pos = 0;
    while (pos < n) {
        std::size_t run_len = detail::find_run(first + pos, last, comp);

        // Extend short runs to min_run using insertion sort.
        if (run_len < min_run) {
            std::size_t force = std::min(min_run, n - pos);
            detail::insertion_sort_range(first + pos, first + pos + force, comp);
            run_len = force;
        }

        stack.push_back({pos, run_len});
        pos += run_len;

        // Merge to maintain the invariant:
        //   stack[i-2].len > stack[i-1].len + stack[i].len
        //   stack[i-1].len > stack[i].len
        while (stack.size() >= 2) {
            auto sz = stack.size();
            bool merged = false;

            if (sz >= 3) {
                auto& a = stack[sz - 3];
                auto& b = stack[sz - 2];
                auto& c = stack[sz - 1];
                if (a.length <= b.length + c.length) {
                    if (a.length < c.length) {
                        // Merge A and B.
                        detail::merge_runs(first + a.start,
                                           first + a.start + a.length,
                                           first + a.start + a.length + b.length,
                                           comp, buf);
                        a.length += b.length;
                        stack.erase(stack.begin() + static_cast<long>(sz - 2));
                    } else {
                        // Merge B and C.
                        detail::merge_runs(first + b.start,
                                           first + b.start + b.length,
                                           first + b.start + b.length + c.length,
                                           comp, buf);
                        b.length += c.length;
                        stack.pop_back();
                    }
                    merged = true;
                } else if (b.length <= c.length) {
                    detail::merge_runs(first + b.start,
                                       first + b.start + b.length,
                                       first + b.start + b.length + c.length,
                                       comp, buf);
                    b.length += c.length;
                    stack.pop_back();
                    merged = true;
                }
            } else {
                // sz == 2
                auto& a = stack[sz - 2];
                auto& b = stack[sz - 1];
                if (a.length <= b.length) {
                    detail::merge_runs(first + a.start,
                                       first + a.start + a.length,
                                       first + a.start + a.length + b.length,
                                       comp, buf);
                    a.length += b.length;
                    stack.pop_back();
                    merged = true;
                }
            }

            if (!merged) break;
        }
    }

    // Final merges: collapse the stack.
    while (stack.size() >= 2) {
        auto sz = stack.size();
        auto& a = stack[sz - 2];
        auto& b = stack[sz - 1];

        if (sz >= 3 && stack[sz - 3].length < b.length) {
            auto& top = stack[sz - 3];
            detail::merge_runs(first + top.start,
                               first + top.start + top.length,
                               first + top.start + top.length + a.length,
                               comp, buf);
            top.length += a.length;
            stack.erase(stack.begin() + static_cast<long>(sz - 2));
        } else {
            detail::merge_runs(first + a.start,
                               first + a.start + a.length,
                               first + a.start + a.length + b.length,
                               comp, buf);
            a.length += b.length;
            stack.pop_back();
        }
    }
}

} // namespace algorithms::sorting::timsort

