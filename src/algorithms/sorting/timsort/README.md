# TimSort (Stable, Adaptive)

This module implements **TimSort**, a *stable*, *adaptive*, O(n log n) comparison sort inspired by Python's built-in sort (designed by Tim Peters, 2002).

Implementation highlights:
- **Natural run detection** — exploits existing ascending/descending runs
- **Minimum run length** computation to balance merge tree
- **Merge stack** with invariant enforcement for balanced merges
- **Insertion sort** for short runs/sub-arrays
- **Stable** merging with buffer optimization (copies the smaller side)

---

## When to use

TimSort is ideal when:
- **Stability** is required and data is often **partially sorted**
- Real-world data typically has natural runs (timestamps, logs, pre-sorted chunks)
- You want **O(n) best-case** on already-sorted data with O(n log n) worst-case guarantee

It is the default sort in Python, Java (for objects), and Android.

---

## API

Header: `include/algorithms/sorting/timsort/timsort.h`

```cpp
#include <algorithms/sorting/timsort/timsort.h>

std::vector<int> a = {5, 3, 1, 4, 2};
algorithms::sorting::timsort::timsort(a.begin(), a.end());
```

Custom comparator:

```cpp
algorithms::sorting::timsort::timsort(a.begin(), a.end(), std::greater<int>{});
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Best-case time | O(n) — already sorted |
| Average time | O(n log n) |
| Worst-case time | O(n log n) |
| Auxiliary space | O(n) |
| Stable | Yes |

---

## Proof / correctness

See [`proof.md`](./proof.md).

