# Fenwick Tree (Binary Indexed Tree)

A **Fenwick tree** (also called **Binary Indexed Tree**, BIT) is a compact data structure that supports:

- **Point updates**: add a value to `A[i]`
- **Prefix sums**: `sum(A[0..r])`
- **Range sums**: `sum(A[l..r])`

All in **O(log N)** time, using **O(N)** memory.

This module implements a classic Fenwick tree for **64-bit integer sums**, with a **0-based public API**.

---

## API

Header: `include/data_structures/range_query/fenwick/fenwick.h`

```cpp
#include <data_structures/range_query/fenwick/fenwick.h>

ds::range_query::fenwick::FenwickTree ft({1, 2, 3, 4});

ft.add(1, 5);                 // A[1] += 5
auto p = ft.prefix_sum(2);    // sum A[0..2]
auto r = ft.range_sum(1, 3);  // sum A[1..3]
```

Public methods:

- `FenwickTree(int n)` — allocate `n` elements initialized to 0
- `FenwickTree(const std::vector<int64_t>& arr)` — build from an array
- `add(i, delta)` — point add
- `prefix_sum(r)` — sum on `[0..r]` (inclusive)
- `range_sum(l, r)` — sum on `[l..r]` (inclusive)

### Input validation behavior

This repo’s range-query structures tend to be defensive:

- `add(i, delta)` is a **no-op** if `i` is out of bounds.
- `prefix_sum(r)` returns **0** for `r < 0`, and clamps `r` to `n-1`.
- `range_sum(l, r)` returns **0** for empty/invalid ranges and clamps to `[0..n-1]`.

---

## How it works (intuition)

This implementation uses the common **0-based Fenwick variant**.

It maintains an internal array `tree[0..n-1]` such that:

- `tree[i]` stores the sum over the segment
  `[(i & (i + 1)) .. i]` (inclusive).

This makes the internal tree ranges easy to walk using bit tricks:

- move *upward* (during point update): `i = i | (i + 1)`
- move *downward* (during prefix sum): `i = (i & (i + 1)) - 1`

### Least-significant bit note

Many references define Fenwick trees with **1-based indexing** and `lsb(i) = i & -i`.
This repo uses 0-based indexing internally, so the equivalent boundary for the segment is computed using:

- `start(i) = i & (i + 1)`

---

## Complexity

- Memory: `O(N)`
- `add`: `O(log N)`
- `prefix_sum`: `O(log N)`
- `range_sum`: `O(log N)`

---

## Proof

See [`proof.md`](./proof.md).
