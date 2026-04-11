# Ternary Search

This module implements **Ternary Search** in three flavors:
1. **Continuous optimization** — find the min/max of a unimodal function on a real interval
2. **Discrete optimization** — find the min/max of a unimodal function on integer indices
3. **Array search** — find a value in a sorted array by trisecting the range

Implementation highlights:
- **Continuous:** converges to within absolute tolerance `eps` (default 1e-9)
- **Discrete:** reduces to a small range, then brute-forces the remainder
- **Array search:** trisects the range each step for O(log₃ n) comparisons
- All variants are header-only, templated on the function/iterator type

---

## When to use

### Continuous / Discrete optimization

Ternary search is the standard technique for:
- **Optimizing unimodal functions** — functions that first decrease then increase (or vice versa)
- **Competitive programming** geometry problems (e.g., minimum distance, maximum area)
- Any problem where you need to find the peak/valley of a function but cannot take derivatives

### Array search (educational)

For searching in sorted arrays, **binary search is strictly better** in practice (2 comparisons per step vs. ternary's 4, despite eliminating 1/3 vs. 1/2 of the range). The array variant is included for completeness.

---

## API

Header: `include/algorithms/sorting/ternary_search/ternary_search.h`

### Continuous optimization

```cpp
#include <algorithms/sorting/ternary_search/ternary_search.h>

namespace ts = algorithms::sorting::ternary_search;

// Find x in [0, 10] that minimizes (x - 3)^2
auto f = [](double x) { return (x - 3.0) * (x - 3.0); };
double x_min = ts::find_min(f, 0.0, 10.0);  // ≈ 3.0

// Find x in [0, 10] that maximizes -(x - 5)^2 + 25
auto g = [](double x) { return -(x - 5.0) * (x - 5.0) + 25.0; };
double x_max = ts::find_max(g, 0.0, 10.0);  // ≈ 5.0
```

### Discrete optimization

```cpp
// Find index in [0, 100] that minimizes |i - 42|
auto h = [](int64_t i) { return std::abs(i - 42); };
int64_t idx = ts::find_min_discrete(h, 0, 100);  // 42
```

### Array search

```cpp
std::vector<int> a = {1, 3, 5, 7, 9, 11, 13};
auto it = ts::ternary_search(a.begin(), a.end(), 7);  // found
```

---

## Complexity

| Variant | Comparisons | Time |
|---------|-------------|------|
| Continuous `find_min`/`find_max` | O(log((hi−lo)/ε)) | O(log((hi−lo)/ε)) function evals |
| Discrete `find_min_discrete`/`find_max_discrete` | O(log₃(hi−lo)) | O(log n) function evals |
| Array `ternary_search` | O(log₃ n) ≈ O(log n) | O(log n) comparisons |
| Space (all variants) | O(1) | |

---

## Comparison with Binary Search

| Property | Binary Search | Ternary Search |
|----------|--------------|----------------|
| Per-step reduction | 1/2 of range | 1/3 of range |
| Comparisons per step | 1–2 | 2 (function evals) or 4 (array) |
| Total comparisons | ~log₂ n | ~2 log₃ n ≈ 1.26 log₂ n |
| Sorted array search | Preferred | Slightly worse |
| Unimodal optimization | Cannot do | Natural fit |

For unimodal optimization, ternary search is the standard approach. For sorted-array lookup, binary search wins.

---

## Proof / correctness

See [`proof.md`](./proof.md).

