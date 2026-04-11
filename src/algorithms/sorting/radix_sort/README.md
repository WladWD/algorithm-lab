# Radix Sort (LSD, Base-256)

This module implements **LSD Radix Sort**, a *stable*, *non-comparison-based* sorting algorithm for integers.

Implementation highlights:
- **Least-Significant-Digit** (LSD) approach — process from the lowest byte upward
- **Base-256** (byte-at-a-time) — only 8 passes needed for 64-bit integers
- Counting sort as the stable subroutine per digit
- **Signed integer** support via min-value offset
- Skips unnecessary upper-byte passes when max value is small

---

## When to use

Radix Sort is ideal when:
- Sorting **integers** (or integer-keyed records) in bulk
- The number of digits `d` is small relative to `log n` — making it faster than O(n log n) sorts
- Stability is desired

For 32-bit integers: 4 passes. For 64-bit: up to 8 passes. In practice, radix sort on large arrays of integers consistently outperforms comparison sorts.

---

## API

Header: `include/algorithms/sorting/radix_sort/radix_sort.h`

```cpp
#include <algorithms/sorting/radix_sort/radix_sort.h>

std::vector<uint64_t> a = {170, 45, 75, 90, 802, 24, 2, 66};
algorithms::sorting::radix_sort::radix_sort(a);
// a = {2, 24, 45, 66, 75, 90, 170, 802}
```

Signed integers:

```cpp
std::vector<int64_t> b = {-5, 3, -1, 0, 7, -3};
algorithms::sorting::radix_sort::radix_sort_signed(b);
// b = {-5, -3, -1, 0, 3, 7}
```

---

## Complexity

| Metric | Bound |
|--------|-------|
| Time | O(d · (n + b)) |
| Space | O(n + b) |
| Stable | Yes |

Where `n` = elements, `d` = digits (bytes), `b` = base (256). For 64-bit integers: d ≤ 8.

---

## Proof / correctness

See [`proof.md`](./proof.md).

