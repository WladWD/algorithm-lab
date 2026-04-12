# Bucket Sort

This module implements **Bucket Sort**, a *distribution-based* sorting algorithm that partitions elements into buckets, sorts each bucket individually, and concatenates the results.

Implementation highlights:
- **Automatic range detection** — finds min/max and distributes uniformly
- **Insertion sort** per bucket (stable, efficient for small buckets)
- Two variants: `bucket_sort` (doubles) and `bucket_sort_int` (int64_t)
- Configurable number of buckets (defaults to n)

---

## When to use

Bucket Sort is ideal when:
- Data is **approximately uniformly distributed** over a known range
- You want **O(n) expected time** (better than comparison sorts)
- Floating-point or integer data with a bounded range

If data is highly clustered, most elements end up in a few buckets and performance degrades to O(n²).

---

## API

Header: `include/algorithms/sorting/bucket_sort/bucket_sort.h`

```cpp
#include <algorithms/sorting/bucket_sort/bucket_sort.h>

namespace bs = algorithms::sorting::bucket_sort;

std::vector<double> a = {0.42, 0.32, 0.33, 0.52, 0.37, 0.47, 0.51};
bs::bucket_sort(a);

std::vector<int64_t> b = {29, 25, 3, 49, 9, 37, 21, 43};
bs::bucket_sort_int(b);
```

---

## Complexity

| Metric | Bound (uniform data) | Worst case |
|--------|---------------------|------------|
| Time | O(n + k) expected | O(n²) |
| Space | O(n + k) | O(n + k) |
| Stable | Yes | Yes |

Where `k` = number of buckets (default: n).

---

## Proof / correctness

See [`proof.md`](./proof.md).

