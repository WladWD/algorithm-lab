# Ternary Search — Proof of Correctness

## 1. Continuous optimization: `find_min`

### Problem

Given a unimodal function `f` on `[lo, hi]` (first strictly decreasing to a unique minimum `m`, then strictly increasing), find `m` to within absolute tolerance `eps`.

### Algorithm

Each iteration:
1. Compute `m1 = lo + (hi - lo) / 3` and `m2 = hi - (hi - lo) / 3`.
2. Evaluate `f(m1)` and `f(m2)`.
3. If `f(m1) < f(m2)`: discard `[m2, hi]`; set `hi = m2`.
4. Otherwise: discard `[lo, m1]`; set `lo = m1`.

### Loop invariant

The minimum point `x*` lies in `[lo, hi]` at the start of each iteration.

### Proof of correctness

**Key lemma:** For a unimodal function with minimum at `x*`, and two probe points `m1 < m2`:
- If `f(m1) < f(m2)`: the minimum `x*` cannot be in `(m2, hi]`. 
  - *Proof:* Suppose `x* > m2`. Since `f` is increasing on `[x*, ...]` wait — since `x*` is the minimum and `m1 < m2`, and both are to the right of `x*`, then `f(m1) <= f(m2)` by the increasing part. But if `x* > m2`, then both `m1` and `m2` are in the decreasing part, so `f(m1) > f(m2)` — contradiction. More precisely:
  - **Case A:** `x* <= m1 < m2`. Both probes are on the increasing side, so `f(m1) <= f(m2)`. Consistent. The minimum is at or before `m1`, so it's in `[lo, m2]`. ✓
  - **Case B:** `m1 < x* <= m2`. The minimum is between the probes. It's in `[lo, m2]`. ✓
  - **Case C:** `m1 < m2 < x*`. Both probes are on the decreasing side, so `f(m1) > f(m2)`. This contradicts `f(m1) < f(m2)`. ✗ — this case is impossible.
  
  Therefore when `f(m1) < f(m2)`, the minimum is in `[lo, m2]` and we can safely set `hi = m2`. ∎

- If `f(m1) >= f(m2)`: by a symmetric argument, the minimum is in `[m1, hi]` and we set `lo = m1`. ∎

### Termination

Each iteration reduces the interval length by a factor of 2/3:
```
(hi - lo)_new = (2/3) * (hi - lo)_old
```
After `k` iterations: `(hi - lo) = (2/3)^k * (hi - lo)_initial`.

The loop terminates when `hi - lo <= eps`, which happens after:
```
k = ⌈log_{3/2}((hi - lo) / eps)⌉ = O(log((hi - lo) / eps))
```
iterations. ∎

### Convergence

When `hi - lo <= eps`, returning `(lo + hi) / 2` gives a point within `eps/2` of the true minimum `x*` (since `x*` is in `[lo, hi]`). ∎

---

## 2. `find_max` — symmetric argument

`find_max` handles unimodal functions that first increase then decrease. The proof is identical with the comparison direction reversed:
- If `f(m1) > f(m2)`: the maximum is in `[lo, m2]`.
- Otherwise: the maximum is in `[m1, hi]`.

---

## 3. Discrete optimization: `find_min_discrete`

### Adaptation

For integer domains, the same trisection logic applies. The key difference is that the range consists of discrete points, and we stop when `hi - lo < 3` (at which point trisection would not produce two distinct interior points). We then brute-force the remaining ≤3 candidates.

### Correctness

The continuous proof's loop invariant carries over: `x*` remains in `[lo, hi]` after each step. The brute-force at the end examines every remaining candidate, guaranteeing we find the true optimum. ∎

### Complexity

Each step reduces the range by a factor of 2/3. The number of steps is `O(log_{3/2} n) = O(log n)`. The final brute-force is O(1). ∎

---

## 4. Array search: `ternary_search` on sorted arrays

### Algorithm

Given a sorted array `A[lo..hi)` and target `value`:
1. Compute `m1 = lo + n/3` and `m2 = lo + 2n/3`.
2. If `A[m1] == value` or `A[m2] == value`: return.
3. If `value < A[m1]`: search `[lo, m1)`.
4. If `value > A[m2]`: search `[m2+1, hi)`.
5. Otherwise: search `[m1+1, m2)`.

### Loop invariant

If `value` exists in the original array, it lies in `[lo, hi)`.

### Proof

At each step, we compare `value` against `A[m1]` and `A[m2]`:
- `value < A[m1]`: since the array is sorted, all elements in `[m1, hi)` are `>= A[m1] > value`. So `value` can only be in `[lo, m1)`. ✓
- `value > A[m2]`: symmetrically, `value` can only be in `(m2, hi)`. ✓
- `A[m1] < value < A[m2]`: (and `value ≠ A[m1]`, `value ≠ A[m2]`). Elements before `m1` are `<= A[m1] < value`, and elements after `m2` are `>= A[m2] > value`. So `value` can only be in `(m1, m2)`. ✓

When `n <= 2`, we brute-force the remaining elements. ∎

### Termination

Each step reduces the range to at most `⌈2n/3⌉` elements (the largest of the three thirds). So after `O(log_{3/2} n)` steps, the range is empty. ∎

### Complexity

`O(log₃ n) ≈ O(log n)` comparisons, `O(1)` space.

**Note:** Despite the same asymptotic complexity as binary search, ternary search on arrays performs **more comparisons** per step (2 function evaluations vs. 1), making it ~1.26x slower in practice. It is included for educational completeness.

---

## References

- CP-Algorithms: "Ternary search" — standard reference for competitive programming.
- T. H. Cormen et al., *Introduction to Algorithms (CLRS)* — analysis of divide-and-conquer recurrences.
- Jon Bentley, *Programming Pearls* — practical comparison of search strategies.

