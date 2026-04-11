# MergeSort (Top-Down, Stable)

This module implements **MergeSort** as a *stable*, *comparison-based* divide-and-conquer sort.

Implementation highlights:
- **Top-down** recursive split with a shared auxiliary buffer (single allocation)
- **Insertion sort** for small sub-arrays (≤16 elements)
- **Early exit** when the two halves are already in order (skip merge)
- **Stability** preserved by preferring left elements on ties

---

## When to use

MergeSort is the go-to choice when:
- **Stable** ordering is required (equal elements keep original order)
- **Guaranteed O(n log n)** worst-case time matters
- Extra O(n) memory is acceptable

If in-place sorting or cache-friendliness is more important, consider HeapSort or QuickSort.

---

## API

Header: `include/algorithms/sorting/mergesort/mergesort.h`

```cpp
#include <algorithms/sorting/mergesort/mergesort.h>

std::vector<int> a = {5, 3, 1, 4, 2};
algorithms::sorting::mergesort::mergesort(a.begin(), a.end());
```

Custom comparator:

```cpp
algorithms::sorting::mergesort::mergesort(a.begin(), a.end(), std::greater<int>{});
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Best-case time | O(n log n) |
| Average time | O(n log n) |
| Worst-case time | O(n log n) |
| Auxiliary space | O(n) |

---

## Proof / correctness

See [`proof.md`](./proof.md).

