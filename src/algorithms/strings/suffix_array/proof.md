# Suffix Array — Proof of Correctness

## Definitions

For string `s[0..n-1]`, the **suffix array** SA satisfies:
```
s[SA[0]..] < s[SA[1]..] < … < s[SA[n-1]..] (lexicographic order)
```

The **LCP array** satisfies `lcp[0] = 0` and:
```
lcp[i] = |LCP(s[SA[i-1]..], s[SA[i]..])|   for i ≥ 1
```

---

## Suffix Array: O(n log² n) prefix-doubling construction

### Algorithm

Maintain a rank array `rank[i]` = rank of suffix `s[i..]` among all suffixes when only the first `k` characters are considered.

```
Initialise rank[i] = s[i]           (character codes, step k = 1)

For k = 1, 2, 4, 8, …, n:
    Sort suffixes by key (rank[i], rank[i+k])   — rank[i+k] = −1 if out of bounds
    Reassign ranks 0..r from the sorted order (equal keys → equal rank)
    If all ranks are distinct: stop early
```

### Correctness

**Invariant:** After the iteration where `k = 2^t`, `rank[i]` is the rank of `s[i..]` among all suffixes when only the first `2^t` characters are compared.

**Base case (k = 1):** Trivially true by initialisation.

**Inductive step:** Assume the invariant holds for `k`. Sorting by `(rank[i], rank[i+k])` sorts by the first `2k` characters: `rank[i]` covers characters 0..k−1, and `rank[i+k]` covers characters k..2k−1 of suffix `i`. Two suffixes with equal 2k-character prefixes receive equal ranks. ✓

**Termination:** Ranks are non-decreasing integers in 0..n−1. If all ranks are distinct at step `k ≥ n/2`, every suffix has a unique rank, giving the true lexicographic order. The loop runs at most `⌈log n⌉` iterations.

### Time complexity: O(n log² n)

Each iteration calls `std::sort` on n elements: O(n log n). There are O(log n) iterations. Total: **O(n log² n)**. ∎

---

## LCP Array: Kasai's O(n) algorithm

### Algorithm

Let `rank[i]` be the inverse of SA (i.e., `rank[SA[i]] = i`).

```
h = 0
for i = 0 to n−1:
    if rank[i] == 0: h = 0; continue
    j = SA[rank[i] − 1]           // preceding suffix in sorted order
    while i+h < n and j+h < n and s[i+h] == s[j+h]: h++
    lcp[rank[i]] = h
    if h > 0: h--
```

### Correctness

**Claim:** `lcp[rank[i]] = LCP(s[i..], s[j..])` where `j = SA[rank[i]−1]`.

This is the definition of the LCP array, so correctness reduces to showing that the inner loop terminates with `h = |LCP(s[i..], s[j..])|`. The loop extends `h` while characters match and stops on the first mismatch — this is correct by construction.

### Time complexity: O(n)

`h` increases by at most 1 per outer iteration, so at most `+n` total increases.
Each inner loop iteration strictly decreases `h` by the amount it was previously increased.
The `h--` at the end of each outer iteration decreases `h` by at most 1 per outer step.
Total decreases ≤ total increases = n. Combined: **O(n)**. ∎

---

## Pattern search: O(m log n)

Given SA, finding all occurrences of pattern `p[0..m-1]` in text `t[0..n-1]` reduces to finding the contiguous range in SA where `t[SA[i]..SA[i]+m-1] == p`.

Two binary searches locate the left and right boundaries in O(m log n) each (each comparison is O(m), and there are O(log n) comparisons per search). ∎

---

## References

- U. Manber and G. Myers, "Suffix arrays: A new method for on-line string searches," SIAM J. Comput., 1993.
- T. Kasai et al., "Linear-time longest-common-prefix computation in suffix arrays and its applications," CPM 2001.
- CP-Algorithms: "Suffix Array" — practical prefix-doubling treatment.
