# Binary Search (lower_bound, upper_bound, equal_range)

This module implements the classic **binary search** family on sorted random-access ranges.

Implementation highlights:
- **Four variants:** `lower_bound`, `upper_bound`, `binary_search` (existence), `equal_range`
- Templated on iterator type and comparator
- Matches the semantics of `std::lower_bound` / `std::upper_bound`

---

## When to use

Binary search is the fundamental technique for:
- **Existence checks** in sorted data — O(log n)
- **Insertion points** — finding where to insert a value to keep order
- **Counting occurrences** — `equal_range` gives the range of matching elements
- **Predicate search** — any monotonic predicate can be binary-searched

---

## API

Header: `include/algorithms/sorting/binary_search/binary_search.h`

```cpp
#include <algorithms/sorting/binary_search/binary_search.h>

namespace bs = algorithms::sorting::binary_search;

std::vector<int> a = {1, 3, 3, 5, 7, 9};

auto it = bs::lower_bound(a.begin(), a.end(), 3);  // points to first 3
auto it2 = bs::upper_bound(a.begin(), a.end(), 3); // points to 5
bool found = bs::binary_search(a.begin(), a.end(), 5); // true
auto [lo, hi] = bs::equal_range(a.begin(), a.end(), 3); // range of 3s
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Time | O(log n) |
| Space | O(1) |

---

## Proof / correctness

See [`proof.md`](./proof.md).

