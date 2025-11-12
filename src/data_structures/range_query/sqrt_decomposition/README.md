# Sqrt (Block) Decomposition — Practical Guide

## Overview

Sqrt decomposition (a.k.a. block decomposition) partitions an array into contiguous blocks of size about `B ≈ √N` and precomputes summary information per block. It offers a simple, cache-friendly tradeoff between precomputation and per-query cost and is easy to implement compared with segment trees or Fenwick trees.

## Problem statement

Given an array `A[0..N-1]`, support operations such as:
- `update(i, val)`: set `A[i] = val` (or add `val`), and
- `query(l, r)`: compute an aggregate over `A[l..r]` (examples: sum, min, gcd).

We want to support both operations faster than O(N) per operation with low implementation complexity.

## Basic idea and formulas

Partition the array into `blockCount = ceil(N / B)` blocks, where the block size `B` is chosen around `sqrt(N)`.

Let `blockId(i) = floor(i / B)` and let `blockSum[b]` store the sum (or other summary) of all elements in block `b`.

For a range-sum query `query(l,r)`, split the interval into three parts:

1. Partial prefix in `blockId(l)` from `l` to the end of that block.
2. Zero or more full blocks between `blockId(l)+1` and `blockId(r)-1` — these are answered by precomputed block summaries.
3. Partial suffix in `blockId(r)` from the start of that block to `r`.

Displayed formula (sum case):

```
query(l, r) = sum_{i=l..min(r, end(blockId(l)))} A[i]
            + sum_{b = blockId(l)+1 .. blockId(r)-1} blockSum[b]
            + sum_{i=max(l, start(blockId(r)))..r} A[i]
```

Choosing `B ≈ sqrt(N)` balances the two work terms (per-query partial scans and number of blocks) and yields ≈ `O(√N)` per query with `O(N)` preprocessing.

## Variants

- Point-update + range-query (classic): update `A[i]` and rebuild `blockSum[blockId(i)]` in O(1) for the value change; queries are `O(B + number_of_full_blocks)`.
- Range-update + point-query: use difference arrays inside blocks or lazy per-block tags.
- Range-update + range-query: more complex — prefer segment tree or Fenwick tree with range-update features.
- Offline queries: when all queries are known in advance, other algorithms (Mo, offline divide-and-conquer) might be preferable for certain problems.

## Complexity

- Preprocessing: `O(N)` to compute block summaries.
- Point update: `O(1)` to update the element and corresponding `blockSum`.
- Range query (sum): `O(B + (rBlock - lBlock))` ≈ `O(B + N/B)`. With `B = sqrt(N)` this is `O(√N)`.

Trade-offs: smaller `B` lowers per-block scanning but increases number of blocks. For typical `N` and uniformly distributed queries, `B = max(1, int(sqrt(N)))` is a practical default.

## Implementation notes and pitfalls

- Choose `B` as `int(sqrt(N))` or tune for cache/constant factors: sometimes `B = 512` or `B = 1024` is faster for large arrays on modern hardware.
- Use integer types large enough for aggregates (e.g., 64-bit for sums).
- If values are updated frequently and queries are many, prefer Fenwick/segment trees for guaranteed `O(log N)` operations.
- For minima or other non-invertible aggregates, block summaries must be recomputed or stored with data structures; the simplest approach recomputes the block when a change occurs (O(B)).

## Pseudocode (point-update + range-sum)

```
initialize(A):
  B = max(1, floor(sqrt(N)))
  blockCount = ceil(N / B)
  blockSum[0..blockCount-1] = 0
  for i in 0..N-1:
    blockSum[blockId(i)] += A[i]

update(i, val): // set A[i] = val
  b = blockId(i)
  blockSum[b] += (val - A[i])
  A[i] = val

query(l, r):
  res = 0
  if blockId(l) == blockId(r):
    for i in l..r: res += A[i]
    return res
  // left partial
  endL = endOfBlock(blockId(l))
  for i in l..endL: res += A[i]
  // full blocks
  for b in blockId(l)+1 .. blockId(r)-1: res += blockSum[b]
  // right partial
  startR = startOfBlock(blockId(r))
  for i in startR..r: res += A[i]
  return res
```

## When to use / alternatives

- Use sqrt decomposition for simple tasks, teaching, and when you want a compact and clear implementation that outperforms naive O(N) for medium-sized `N`.
- For guaranteed logarithmic performance, use segment trees or Fenwick trees (BIT). For offline range queries consider Mo's algorithm.