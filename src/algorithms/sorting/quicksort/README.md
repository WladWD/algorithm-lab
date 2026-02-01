# QuickSort (Three-way Partition)

This module implements **QuickSort** as an *in-place*, *unstable* comparison sort.

Implementation highlights:
- **Median-of-three pivot** selection
- **Three-way partitioning** (Dutch National Flag) for good behavior on inputs with many duplicates
- A recursion **depth guard** (introsort-style): if recursion gets too deep, it falls back to `std::sort`

---

## When to use

QuickSort is a great default choice when:
- you need an in-place sort
- average-case performance matters and occasional worst-case is acceptable

If you need stable ordering, prefer a stable sort (e.g., merge sort).

---

## API

Header: `include/algorithms/sorting/quicksort/quicksort.h`

```cpp
#include <algorithms/sorting/quicksort/quicksort.h>

std::vector<int> a = {3, 1, 2, 2, 5};
algorithms::sorting::quicksort::quicksort(a.begin(), a.end());
```

Custom comparator:

```cpp
algorithms::sorting::quicksort::quicksort(a.begin(), a.end(), std::greater<int>{});
```

---

## Complexity

Let `n` be the number of elements.

- Average time: `O(n log n)`
- Worst-case time (classic quicksort): `O(n^2)`
- Extra memory: `O(log n)` expected recursion (this implementation also uses tail-recursion elimination)

---

## Behavior notes

- Unstable: equal elements may change relative order.
- Many duplicates: handled efficiently by three-way partitioning.
- Worst-case avoidance: a recursion depth guard switches to `std::sort` when depth exceeds ~`2*log2(n)`.

---

## Proof / correctness

See [`proof.md`](./proof.md).

