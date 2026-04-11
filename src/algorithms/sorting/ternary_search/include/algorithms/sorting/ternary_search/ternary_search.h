#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <type_traits>

namespace algorithms::sorting::ternary_search {

// Ternary Search — find the minimum (or maximum) of a unimodal function,
// or search for a value in a sorted array by trisecting the range.
//
// A function f is *unimodal* on [lo, hi] if there exists a point m such that
// f is strictly decreasing on [lo, m] and strictly increasing on [m, hi]
// (for finding a minimum), or vice-versa (for finding a maximum).
//
// Ternary search narrows the range by evaluating f at two interior points
// that divide the interval into three equal parts, discarding one third
// each iteration.
//
// Complexity: O(log n) for discrete, O(log((hi-lo)/eps)) for continuous.

// ========================= Continuous variant =========================

/// Find the value of x in [lo, hi] that minimizes f(x).
/// `f` must be unimodal (first decreasing, then increasing) on [lo, hi].
/// `eps` is the absolute tolerance for convergence.
/// Returns the x that approximately minimizes f.
template <typename F>
double find_min(F&& f, double lo, double hi, double eps = 1e-9) {
    while (hi - lo > eps) {
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;
        if (f(m1) < f(m2)) {
            hi = m2;
        } else {
            lo = m1;
        }
    }
    return (lo + hi) / 2.0;
}

/// Find the value of x in [lo, hi] that maximizes f(x).
/// `f` must be unimodal (first increasing, then decreasing) on [lo, hi].
/// Returns the x that approximately maximizes f.
template <typename F>
double find_max(F&& f, double lo, double hi, double eps = 1e-9) {
    while (hi - lo > eps) {
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;
        if (f(m1) > f(m2)) {
            hi = m2;
        } else {
            lo = m1;
        }
    }
    return (lo + hi) / 2.0;
}

// ======================== Discrete variant ============================

/// Find the index in [lo, hi] (inclusive) that minimizes f(index).
/// `f` must be unimodal on integer indices.
/// Returns the index that minimizes f.
template <typename F>
int64_t find_min_discrete(F&& f, int64_t lo, int64_t hi) {
    while (hi - lo >= 3) {
        int64_t m1 = lo + (hi - lo) / 3;
        int64_t m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) {
            hi = m2;
        } else {
            lo = m1;
        }
    }
    // Brute-force the remaining small range.
    int64_t best = lo;
    for (int64_t i = lo + 1; i <= hi; ++i) {
        if (f(i) < f(best)) best = i;
    }
    return best;
}

/// Find the index in [lo, hi] (inclusive) that maximizes f(index).
/// `f` must be unimodal on integer indices.
template <typename F>
int64_t find_max_discrete(F&& f, int64_t lo, int64_t hi) {
    while (hi - lo >= 3) {
        int64_t m1 = lo + (hi - lo) / 3;
        int64_t m2 = hi - (hi - lo) / 3;
        if (f(m1) > f(m2)) {
            hi = m2;
        } else {
            lo = m1;
        }
    }
    int64_t best = lo;
    for (int64_t i = lo + 1; i <= hi; ++i) {
        if (f(i) > f(best)) best = i;
    }
    return best;
}

// =================== Array search variant =============================

/// Search for `value` in a sorted range [first, last) by trisecting.
/// Returns iterator to an element equal to `value`, or `last` if not found.
/// Complexity: O(log₃ n) comparisons ≈ O(log n).
///
/// Note: Binary search is generally faster in practice (fewer comparisons
/// per iteration). This is included for completeness and educational purposes.
template <class RandomIt, class T>
RandomIt ternary_search(RandomIt first, RandomIt last, const T& value) {
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                    typename std::iterator_traits<RandomIt>::iterator_category>,
                  "ternary_search requires RandomAccessIterator");

    auto lo = first;
    auto hi = last;

    while (lo < hi) {
        auto n = hi - lo;
        if (n <= 2) {
            // Brute-force tiny ranges.
            for (auto it = lo; it != hi; ++it) {
                if (*it == value) return it;
            }
            return last;
        }

        auto m1 = lo + n / 3;
        auto m2 = lo + 2 * n / 3;

        if (*m1 == value) return m1;
        if (*m2 == value) return m2;

        if (value < *m1) {
            hi = m1;
        } else if (value > *m2) {
            lo = m2 + 1;
        } else {
            lo = m1 + 1;
            hi = m2;
        }
    }

    return last;
}

} // namespace algorithms::sorting::ternary_search

