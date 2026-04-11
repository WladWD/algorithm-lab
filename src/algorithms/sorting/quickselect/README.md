# QuickSelect (k-th Smallest Element)

This module implements **QuickSelect**, an *in-place* selection algorithm based on partitioning.

Implementation highlights:
- **Three-way partition** (Dutch National Flag) with median-of-three pivot
- **In-place**, iterative (tail-call eliminated)
- Semantics match `std::nth_element`: after the call, `*nth` is the correct element

---

## When to use

QuickSelect is ideal when:
- You need the **k-th smallest** (or largest) element without fully sorting
- **O(n) expected time** is acceptable
- O(1) extra memory is important

If worst-case O(n) is required, consider the Median of Medians algorithm (more complex, higher constant).

---

## API

Header: `include/algorithms/sorting/quickselect/quickselect.h`

```cpp
#include <algorithms/sorting/quickselect/quickselect.h>

namespace qs = algorithms::sorting::quickselect;

std::vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6};
qs::quickselect(a.begin(), a.begin() + 3, a.end());
// a[3] is now the 4th smallest element (0-indexed)
// Elements before a[3] are <= a[3], elements after are >= a[3]
```

Custom comparator:

```cpp
qs::quickselect(a.begin(), a.begin() + 0, a.end(), std::greater<int>{});
// a[0] is now the largest element
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Expected time | O(n) |
| Worst-case time | O(n²) |
| Space | O(1) |

---

## Proof / correctness

See [`proof.md`](./proof.md).

