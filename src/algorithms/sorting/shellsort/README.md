# ShellSort (Ciura Gap Sequence)

This module implements **ShellSort**, an *in-place*, *unstable* comparison sort that generalizes insertion sort by allowing exchanges of elements far apart.

Implementation highlights:
- **Ciura gap sequence** (empirically optimal), extended by factor 2.25 for large inputs
- Falls back to standard insertion sort on the final gap-1 pass
- Truly **in-place** — O(1) auxiliary memory

---

## When to use

ShellSort is a good choice when:
- You need an **in-place** sort with **no recursion** (e.g., embedded systems, stack-limited environments)
- Data is **nearly sorted** — ShellSort's early gap passes fix long-range disorder quickly
- Simplicity of implementation matters more than worst-case guarantees

For guaranteed O(n log n) worst case, prefer HeapSort or MergeSort.

---

## API

Header: `include/algorithms/sorting/shellsort/shellsort.h`

```cpp
#include <algorithms/sorting/shellsort/shellsort.h>

std::vector<int> a = {5, 3, 1, 4, 2};
algorithms::sorting::shellsort::shellsort(a.begin(), a.end());
```

Custom comparator:

```cpp
algorithms::sorting::shellsort::shellsort(a.begin(), a.end(), std::greater<int>{});
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Best-case time | O(n log n) (Ciura) |
| Average time | O(n^(4/3)) empirical |
| Worst-case time | O(n^(3/2)) (Ciura), O(n²) (bad sequences) |
| Auxiliary space | O(1) |
| Stable | No |

---

## Proof / correctness

See [`proof.md`](./proof.md).

