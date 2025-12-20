# Fenwick Tree (BIT) — Correctness Notes

This note matches the implementation in `src/fenwick.cpp`, which uses the common **0-based** Fenwick (BIT) variant.

## Definitions and invariant

We store an internal array `tree[0..N-1]`.

Define:

- `start(i) = i & (i + 1)`

> **Invariant:** `tree[i] = sum_{k = start(i) .. i} A[k]`.

So each `tree[i]` stores the sum of a segment ending at `i`, and the segment length is a power of two.

---

## Prefix sum query

We want the prefix sum:

`P(r) = sum_{k=0..r} A[k]`.

Algorithm (as in code):

```
res = 0
i = r
while i >= 0:
  res += tree[i]
  i = (i & (i + 1)) - 1
return res
```

### Why it works

At each step, we add `tree[i]`, which by the invariant equals:

`sum(A[start(i) .. i])`.

Then we jump to:

`i' = start(i) - 1`.

That means the next segment (if any) ends just before the segment we just consumed.

**Key property:** the sequence of segments

`[start(i) .. i], [start(i') .. i'], ...`

are **disjoint** and their union is exactly `[0..r]`. Therefore, the loop returns `P(r)`.

---

## Point update

Operation: `A[pos] += delta`.

We must update all `tree[i]` whose covered segment contains `pos`, i.e. all `i` such that:

`start(i) <= pos <= i`.

Algorithm (as in code):

```
i = pos
while i < N:
  tree[i] += delta
  i = i | (i + 1)
```

### Why it visits exactly the affected nodes

The transition `i := i | (i + 1)` moves upward to the next index whose segment strictly expands and still contains `pos`.

- It never skips a required node (every ancestor segment containing `pos` is reached).
- It never includes an unrelated node (the constructed ancestors are precisely the ones whose segments cover `pos`).

So every segment sum that depends on `A[pos]` gets adjusted by `delta`, preserving the invariant.

---

## Range sum

Range sum on `[l..r]` is computed by prefix sums:

`sum(l..r) = P(r) - P(l - 1)`.

Correctness follows directly from prefix correctness.
