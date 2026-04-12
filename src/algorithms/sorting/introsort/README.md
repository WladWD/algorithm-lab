# IntroSort (QuickSort + HeapSort + InsertionSort Hybrid)

This module implements **IntroSort**, the hybrid sorting strategy used by most `std::sort` implementations (designed by David Musser, 1997).

Implementation highlights:
- **QuickSort** with median-of-three pivot for the main work
- **HeapSort** fallback when recursion depth exceeds 2·log₂(n) — guarantees O(n log n)
- **Insertion sort** for small partitions (≤16 elements)
- Tail-recursion elimination (recurse on smaller partition, loop on larger)

---

## When to use

IntroSort is the **universal default**:
- Fast average case from QuickSort
- Guaranteed O(n log n) worst case from HeapSort fallback
- Efficient on small arrays from insertion sort finish
- This is what `std::sort` typically does under the hood

Use MergeSort/TimSort if you need stability.

---

## API

Header: `include/algorithms/sorting/introsort/introsort.h`

```cpp
#include <algorithms/sorting/introsort/introsort.h>

std::vector<int> a = {5, 3, 1, 4, 2};
algorithms::sorting::introsort::introsort(a.begin(), a.end());
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Best-case time | O(n log n) |
| Average time | O(n log n) |
| Worst-case time | O(n log n) — guaranteed |
| Auxiliary space | O(log n) stack |
| Stable | No |

---

## Proof / correctness

See [`proof.md`](./proof.md).

