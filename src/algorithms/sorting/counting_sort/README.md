# Counting Sort (Stable, Non-Comparison)

This module implements **Counting Sort**, a *stable*, *non-comparison-based* sorting algorithm for non-negative integers.

Implementation highlights:
- **Stable** placement via prefix-sum + reverse traversal
- O(n + k) time and space, where k is the range of input values
- Two overloads: auto-detect max, or caller-supplied max

---

## When to use

Counting Sort is ideal when:
- Input consists of **non-negative integers** in a known, bounded range
- The range `k` is roughly O(n) — making it **linear** in practice
- **Stability** is needed (e.g., as a subroutine of Radix Sort)

If the range is much larger than `n`, consider Radix Sort or a comparison-based sort instead.

---

## API

Header: `include/algorithms/sorting/counting_sort/counting_sort.h`

```cpp
#include <algorithms/sorting/counting_sort/counting_sort.h>

std::vector<int64_t> a = {4, 2, 2, 8, 3, 3, 1};
algorithms::sorting::counting_sort::counting_sort(a);
// a = {1, 2, 2, 3, 3, 4, 8}
```

With explicit upper bound:

```cpp
algorithms::sorting::counting_sort::counting_sort(a, 10);
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Time | O(n + k) |
| Space | O(n + k) |
| Stable | Yes |

Where `n` = number of elements, `k` = max element value + 1.

---

## Proof / correctness

See [`proof.md`](./proof.md).

