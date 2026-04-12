# Exponential Search

This module implements **Exponential Search** (also called *galloping search* or *doubling search*), which finds an element in a sorted array by first exponentially expanding a bound, then binary-searching within it.

Implementation highlights:
- **Three variants:** exact search, existence check, and lower-bound
- **O(log i)** time where `i` is the position of the target — optimal when the target is near the front
- Falls back to standard binary search in the bounded range

---

## When to use

Exponential Search is ideal when:
- The target is expected to be **near the beginning** of a large sorted array
- The array is **unbounded** or its size is unknown (e.g., streaming data, infinite sequences)
- You want **output-sensitive** search: O(log i) instead of O(log n)

Use Binary Search if you don't expect the target to be near the front, since both are O(log n) worst case but binary search has a smaller constant.

---

## API

Header: `include/algorithms/sorting/exponential_search/exponential_search.h`

```cpp
#include <algorithms/sorting/exponential_search/exponential_search.h>

namespace es = algorithms::sorting::exponential_search;

std::vector<int> a = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

// Exact search
auto it = es::exponential_search(a.begin(), a.end(), 6);  // found

// Existence check
bool found = es::exponential_search_exists(a.begin(), a.end(), 6);  // true

// Lower bound (first element >= 7)
auto lb = es::exponential_lower_bound(a.begin(), a.end(), 7);  // points to 8
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Time | O(log i) where i = position of target |
| Worst-case time | O(log n) |
| Space | O(1) |

---

## Proof / correctness

See [`proof.md`](./proof.md).

