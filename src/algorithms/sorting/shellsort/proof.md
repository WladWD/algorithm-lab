# ShellSort — Proof of Correctness

## Algorithm sketch

ShellSort performs multiple passes of insertion sort with decreasing *gap* values. For each gap `g`, it g-sorts the array: elements spaced `g` apart form interleaved subsequences, each sorted by insertion sort. The final pass uses gap 1, which is a standard insertion sort.

---

## Correctness

**Claim:** After ShellSort completes, the array is fully sorted.

**Proof:** The final gap in any valid gap sequence is 1. A gap-1 pass is a standard insertion sort over the entire array, which produces a sorted result regardless of the initial ordering. ∎

The earlier passes with larger gaps serve only to *pre-sort* the data, reducing the work needed by the final gap-1 pass.

---

## Key property: h-sorting preserves g-sorting

**Theorem (Shell, 1959):** An array that is g-sorted remains g-sorted after being h-sorted, for any h.

**Proof sketch:** After g-sorting, every subsequence `A[i], A[i+g], A[i+2g], ...` is sorted. When we h-sort, we rearrange elements within h-spaced subsequences. Consider any g-spaced subsequence after h-sorting. The h-sort moves elements to reduce h-disorder, but because it processes h-interleaved subsequences independently, it cannot increase g-disorder — any g-spaced pair that was in order remains in order or is further improved.

This property is crucial: it means the pre-sorting done by larger gaps is never undone by smaller gaps.

---

## Why larger gaps help

Consider an array like `[n, n-1, ..., 2, 1]` (reverse sorted). Standard insertion sort would require O(n²) comparisons. But a single pass with gap `n/2` brings most elements close to their final positions, dramatically reducing the work for subsequent passes.

More formally, after a gap-g pass, no element is more than `g-1` positions away from its final position within its g-subsequence. As gaps decrease, the array becomes increasingly "nearly sorted," and insertion sort on nearly-sorted data runs in near-linear time.

---

## Gap sequence: Ciura

This implementation uses the **Ciura gap sequence**: `1, 4, 10, 23, 57, 132, 301, 701, 1750, ...`, extended by multiplying by 2.25 for larger inputs.

This sequence was determined empirically by Marcin Ciura (2001) to minimize the average number of comparisons for arrays up to several thousand elements. It outperforms Shell's original (`n/2, n/4, ...`), Hibbard's (`2^k - 1`), and Sedgewick's sequences in practice.

---

## Complexity analysis

### Time

The exact complexity of ShellSort depends on the gap sequence. Known bounds:

| Sequence | Worst case | Average case |
|----------|-----------|-------------|
| Shell (n/2^k) | O(n²) | O(n²) |
| Hibbard (2^k - 1) | O(n^(3/2)) | — |
| Sedgewick | O(n^(4/3)) | — |
| Ciura (empirical) | ≈ O(n^(4/3)) | ≈ O(n^(7/6)) empirical |

No gap sequence is known to achieve O(n log n) worst case for ShellSort.

### Space: O(1)

ShellSort is fully in-place. Only a constant number of variables are used.

---

## Termination

Each gap pass is a standard insertion sort on interleaved subsequences (finitely many elements, finitely many comparisons). The number of gap passes is O(log n) (since gaps grow geometrically). ∎

---

## References

- D. L. Shell, "A high-speed sorting procedure," Communications of the ACM, 1959.
- M. Ciura, "Best increments for the average case of Shellsort," LNCS 2161, 2001.
- R. Sedgewick, "Analysis of Shellsort and related algorithms," ESA 1996.

