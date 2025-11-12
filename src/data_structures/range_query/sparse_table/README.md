# Range Minimum Query (RMQ) — Professional Guide

## Overview

Range Minimum Query (RMQ) asks: given an array `A[0..N-1]`, answer queries asking for the index (or value) of the minimum element on a subarray `A[l..r]`. RMQ is a building block for many problems (LCA, pattern matching, NGS data processing, etc.).

Two common variants:
- Static RMQ: the array is fixed and only queries are performed.
- Dynamic RMQ: the array can be updated (point updates) between queries.

Choose algorithm according to whether updates are required and the desired query latency.

## Problem statement

Given array `A[0..N-1]` and queries `(l, r)` with `0 ≤ l ≤ r < N`, compute

```
RMQ(l, r) = argmin_{i ∈ [l, r]} A[i]
```

Return either the index `i` of the minimum element or the value `A[i]` itself.

If there are ties (equal values), define a stable tie-breaker (e.g., smaller index) and implement comparisons accordingly.

## Sparse table (static RMQ — O(1) queries)

Use when the array is read-only and many queries are required.

Precompute `st[k][i]` — the index of the minimum on the interval of length `2^k` starting at `i`.

Preprocessing (build):
```
for i in 0..N-1: st[0][i] = i
for k in 1..K:    // K = floor(log2(N))
  for i in 0..N - 2^k:
    left = st[k-1][i]
    right = st[k-1][i + 2^(k-1)]
    st[k][i] = (A[left] <= A[right]) ? left : right
```

Query: let `len = r - l + 1`, `k = floor(log2(len))`.
```
i1 = st[k][l]
i2 = st[k][r - 2^k + 1]
return (A[i1] <= A[i2]) ? i1 : i2
```

Complexity:
- Build: `O(N log N)` time, `O(N log N)` memory.
- Query: `O(1)` time (two comparisons).

Pros: fastest queries for static arrays and very simple implementation.
Cons: no updates.


## Complexity summary

```
Algorithm         | Preprocess      | Query        | Update
-------------------------------------------------------------
Sparse table      | O(N log N)      | O(1)         | —
Cartesian+LCA     | O(N) (advanced) | O(1)         | —
Segment tree      | O(N)            | O(log N)     | O(log N)
Fenwick (BIT)     | O(N)            | O(log N)     | O(log N)  (not suitable for RMQ)
```

Notes: Fenwick trees (BIT) are ideal for prefix-sum problems; they are not suitable for RMQ without complex adaptations.

## Pseudocode and C++ skeletons

### Sparse table (static RMQ) — C++ sketch

```cpp
// build indices: st[k][i] stores index of min in [i, i+2^k-1]
int K = floor(log2(N));
vector<int> lg(N+1);
for (int i = 2; i <= N; ++i) lg[i] = lg[i>>1] + 1;
vector<vector<int>> st(K+1, vector<int>(N));
for (int i = 0; i < N; ++i) st[0][i] = i;
for (int k = 1; k <= K; ++k) {
  int len = 1 << k;
  for (int i = 0; i + len <= N; ++i) {
    int a = st[k-1][i];
    int b = st[k-1][i + (1 << (k-1))];
    st[k][i] = (A[a] <= A[b]) ? a : b;
  }
}

// query(l,r)
int len = r - l + 1;
int k = lg[len];
int i1 = st[k][l];
int i2 = st[k][r - (1<<k) + 1];
return (A[i1] <= A[i2]) ? i1 : i2;
```

## References

- Bender, M. A., & Farach-Colton, M. (2000). The LCA problem revisited. Latin American Theoretical Informatics Symposium. (Linear-time RMQ algorithm)
- Competitive programming resources and CP-algorithms: sparse table, segment tree, Mo's algorithm and RMQ variants.
