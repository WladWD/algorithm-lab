# Interpolation Search

This module implements **Interpolation Search**, a search algorithm for sorted arrays of numeric values that estimates the target's position via linear interpolation.

Implementation highlights:
- **Three variants:** exact search, existence check, and lower-bound
- **O(log log n) expected** time on uniformly distributed data
- Falls back gracefully to O(n) on adversarial distributions
- Floating-point interpolation with clamping guards for robustness

---

## When to use

Interpolation Search excels when:
- Data is **sorted** and values are **approximately uniformly distributed** (e.g., timestamps, sequential IDs, sensor readings)
- The value type is **numeric** (integers or floating-point)
- You want **better-than-binary-search** expected performance on large, uniform datasets

Use **Binary Search** instead when:
- Data distribution is unknown or highly non-uniform (exponential, Zipf, etc.)
- You need guaranteed O(log n) worst case
- Values are non-numeric (strings, custom types)

---

## API

Header: `include/algorithms/sorting/interpolation_search/interpolation_search.h`

```cpp
#include <algorithms/sorting/interpolation_search/interpolation_search.h>

namespace is = algorithms::sorting::interpolation_search;

std::vector<int> a = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

// Exact search: returns iterator to 50, or a.end() if not found
auto it = is::interpolation_search(a.begin(), a.end(), 50);

// Existence check
bool found = is::interpolation_search_exists(a.begin(), a.end(), 50); // true

// Lower bound: first element >= 45
auto lb = is::interpolation_lower_bound(a.begin(), a.end(), 45); // points to 50
```

---

## Complexity

| Metric | Uniform distribution | Worst case |
|--------|---------------------|------------|
| Time | O(log log n) | O(n) |
| Space | O(1) | O(1) |

---

## Comparison with Binary Search

| Property | Binary Search | Interpolation Search |
|----------|--------------|---------------------|
| Probe strategy | Always midpoint | Linear interpolation on values |
| Expected time (uniform) | O(log n) | O(log log n) |
| Worst-case time | O(log n) | O(n) |
| Works on non-numeric types | Yes | No (needs arithmetic) |
| Distribution-sensitive | No | Yes |

---

## Proof / correctness

See [`proof.md`](./proof.md).

