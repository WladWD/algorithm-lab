# Median of Medians (BFPRT Algorithm)

This module implements the **Median of Medians** deterministic selection algorithm (Blum, Floyd, Pratt, Rivest, Tarjan — 1973), which finds the k-th smallest element in **worst-case O(n)** time.

Implementation highlights:
- **Groups of 5** — divide, sort each group, take medians
- **Recursive pivot selection** guarantees a balanced partition
- **Same interface** as `std::nth_element` / QuickSelect
- Worst-case O(n), unlike QuickSelect's O(n²) worst case

---

## When to use

Use Median of Medians when:
- **Worst-case O(n)** guarantee is required (e.g., real-time systems, adversarial inputs)
- You cannot tolerate QuickSelect's O(n²) worst case

In practice, QuickSelect with median-of-three is usually faster (lower constant factor). Use MoM when you need the worst-case guarantee.

---

## API

Header: `include/algorithms/sorting/median_of_medians/median_of_medians.h`

```cpp
#include <algorithms/sorting/median_of_medians/median_of_medians.h>

namespace mom = algorithms::sorting::median_of_medians;

std::vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6, 5};
mom::select(a.begin(), a.begin() + 4, a.end());
// a[4] is now the 5th smallest element (0-indexed)
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Worst-case time | O(n) — guaranteed |
| Expected time | O(n) |
| Space | O(log n) recursion stack |
| Stable | No |

---

## Proof / correctness

See [`proof.md`](./proof.md).

