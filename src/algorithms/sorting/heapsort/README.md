# HeapSort (In-Place, Unstable)

This module implements **HeapSort** as an *in-place*, *unstable* comparison sort.

Implementation highlights:
- **Bottom-up heap construction** in O(n)
- **Sift-down** based extraction for O(n log n) total
- Truly **in-place** — zero auxiliary memory beyond the input array

---

## When to use

HeapSort is the best choice when:
- **Guaranteed O(n log n)** worst-case time is required
- **O(1) extra memory** is critical (embedded systems, memory-constrained environments)
- Stability is not needed

If stability matters, use MergeSort. If average-case speed matters more than worst-case guarantees, use QuickSort.

---

## API

Header: `include/algorithms/sorting/heapsort/heapsort.h`

```cpp
#include <algorithms/sorting/heapsort/heapsort.h>

std::vector<int> a = {5, 3, 1, 4, 2};
algorithms::sorting::heapsort::heapsort(a.begin(), a.end());
```

Custom comparator:

```cpp
algorithms::sorting::heapsort::heapsort(a.begin(), a.end(), std::greater<int>{});
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Best-case time | O(n log n) |
| Average time | O(n log n) |
| Worst-case time | O(n log n) |
| Auxiliary space | O(1) |

---

## Proof / correctness

See [`proof.md`](./proof.md).

