# Mo's Algorithm — Offline Range Queries

An offline technique that answers range queries over an array by reordering queries to minimize work per query.

---

## Overview

Mo's algorithm answers many static range queries (no point updates) over a fixed array by processing queries in a carefully chosen order. It maintains a sliding window `[L,R]` and updates a lightweight data structure with `add(index)` / `remove(index)` operations as the window moves. By sorting queries into blocks, the total number of add/remove operations is reduced compared to answering each query independently.

## Problem statement

Given an array `A[0..N-1]` and `Q` queries of the form `(l, r)` (0-based, inclusive), compute some associative property over the subarray `A[l..r]` for each query (examples: sum, frequency-based distinct count, gcd, xor).

Mo's algorithm is applicable when:
- queries are known offline (you can reorder them),
- the query answer can be updated when extending/shrinking the current range via `add(i)` / `remove(i)` operations that run in small (preferably O(1)) time.

## Algorithm idea (block decomposition)

Partition indices into blocks of size `B`. The standard choice is

```text
B = ⌈√N⌉
```

Sort queries by `(block(l), r)` where `block(l) = ⌊l / B⌋`. The comparison ties are typically resolved by `r` ascending. After sorting, process queries in that order while maintaining a current window `[curL, curR]`. For each new query `(l, r)` move `curL` and `curR` using repeated calls to `add`/`remove` until `[curL,curR] == [l,r]`, then record the current answer.

This ordering keeps movement of `L` and `R` small on average, yielding improved total complexity.

## Query ordering and optimizations

Basic comparator (static Mo):

- Primary key: `block(l) = l / B`
- Secondary key: `r` (ascending)

A common micro-optimization is the "odd-even" or "block parity" trick: when `block(l)` is odd, sort by `r` descending instead of ascending. That reduces jumps of `R` between consecutive queries within the same `L` block.

Other improvements:
- Hilbert order: replace block-based comparator with Hilbert curve order of `(l, r)`; this often reduces total movement and improves constant factors.
- Adaptive block size: choose `B = max(1, int(N / sqrt(Q)))` or tune for `add`/`remove` cost.

## Complexity and tuning

Let `cost_add` and `cost_remove` be the costs of a single `add`/`remove` call. With the standard block ordering and `B = ⌈√N⌉` the algorithm achieves roughly

```
Total cost ≈ O((N + Q) * (cost_add + cost_remove) * √N)
```

More practically, the number of moves is `O((N + Q) * √N)` in the worst case. If `Q` is much larger than `N`, using `B = max(1, int(N / sqrt(Q)))` can give better constants. When using Hilbert order, the empirical complexity improves though theoretical worst-case bounds remain similar.

## Implementation notes

- Use 0-based inclusive intervals `[l, r]`.
- Maintain `curL` and `curR` where the current window contains elements `A[curL..curR]`. A common convention:
  - Start with `curL = 0, curR = -1` (empty window).
  - `add(i)` brings index `i` into the data structure; `remove(i)` deletes index `i`.
- Implement `add`/`remove` as small, inlined functions; these dominate runtime.
- Keep answers in an array `ans[Q]` and fill them after each query is processed.
- Avoid heavy STL overhead inside hot `add`/`remove` (prefer arrays and counters for frequency-based measures).

### Typical comparator (C++ style)

```cpp
struct Query { int l, r, idx; };
int B = int(sqrt(N));
std::sort(queries.begin(), queries.end(), [&](const Query &x, const Query &y) {
  int bx = x.l / B, by = y.l / B;
  if (bx != by) return bx < by;
  if (bx & 1) return x.r > y.r; // odd block -> descending r
  else return x.r < y.r;       // even block -> ascending r
});
```

## Pseudocode
```
function processQueries(queries, A):
  B = max(1, floor(sqrt(N)))
  sort queries by (block(l)=l/B, r) with odd-even tweak
  curL = 0; curR = -1
  for q in queries:
    while curL > q.l: curL--; add(curL)
    while curR < q.r: curR++; add(curR)
    while curL < q.l: remove(curL); curL++
    while curR > q.r: remove(curR); curR--
    ans[q.idx] = current_answer()
  return ans
```

## Examples

- Range sum: maintain `current_sum` with `add(i) => current_sum += A[i]`, `remove(i) => current_sum -= A[i]`.
- Distinct count: maintain frequency array `freq[value]` and `distinct` counter (increment when `freq` becomes 1, decrement when becomes 0).
- Frequency-based queries (k-th frequency, majority, etc.) can be supported if `add`/`remove` are cheap.

## When to use / caveats

- Use Mo when queries are offline and `add`/`remove` are cheap (O(1) or near-constant).
- For online queries use segment tree / Fenwick tree (BIT).
- Mo does not handle frequent point updates (there are variants: Mo with updates / Mo's on trees). For updates prefer segment trees or dedicated range-update data structures.
- If the underlying data or value range is large, compress values or use unordered maps with care (may slow down hot loops).

---