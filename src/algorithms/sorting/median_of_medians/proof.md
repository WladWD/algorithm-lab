# Median of Medians — Proof of Correctness

## Algorithm sketch (BFPRT)

Given a range `[first, last)` and a target position `nth`:

1. **Divide** into groups of 5. Sort each group (O(1) per group).
2. **Collect medians** (the middle element of each sorted group).
3. **Recursively** find the median of these medians (the "pivot").
4. **Partition** the full range around this pivot.
5. If `nth` equals the pivot position, done. Otherwise **recurse** on the appropriate side.

---

## Partition correctness

Same as QuickSort/QuickSelect: after partitioning around pivot value `p`, all elements at indices < pivot_pos are < p, and all at indices > pivot_pos are > p. See QuickSort proof.md.

---

## Selection correctness

**Claim:** After `select(first, nth, last)`, the element at `nth` is the same as in the fully sorted array.

**Proof by strong induction on range size:**

- **Base case (n ≤ 5):** Insertion sort produces a fully sorted sub-range. The element at `nth` is correct.
- **Inductive step:** After partitioning around the pivot at position `p`:
  - If `nth == p`: done (pivot is at its correct sorted position).
  - If `nth < p`: recurse on `[first, p)` — a strictly smaller range. By induction, the element at `nth` is correct within this range. Since all elements in `[p, last)` are ≥ pivot, the global position is correct.
  - If `nth > p`: symmetric argument on `[p+1, last)`.

∎

---

## Why groups of 5 guarantee O(n)

### The pivot quality guarantee

The median of medians (MoM) is the median of ⌈n/5⌉ group medians. Among these ⌈n/5⌉ medians:
- Half (≈ n/10) are ≤ MoM.
- Each such median has 2 elements below it in its group (since it's the median of 5).

So at least `3 × n/10 = 3n/10` elements are ≤ MoM. By symmetry, at least `3n/10` elements are ≥ MoM.

This means the partition is at least **30/70** (worst case), never worse. The larger side has at most `7n/10` elements.

### The recurrence

```
T(n) = T(n/5) + T(7n/10) + O(n)
```

Where:
- `T(n/5)`: finding the median of ⌈n/5⌉ medians recursively.
- `T(7n/10)`: recursing on the larger partition side.
- `O(n)`: partitioning + sorting groups of 5.

### Solving the recurrence

Guess `T(n) ≤ cn` for some constant `c`:
```
T(n) ≤ c·(n/5) + c·(7n/10) + an
     = cn·(1/5 + 7/10) + an
     = cn·(9/10) + an
     = (9c/10 + a)·n
```

For this to be ≤ cn, we need `9c/10 + a ≤ c`, i.e., `a ≤ c/10`, so `c ≥ 10a`.

Such a `c` exists, confirming `T(n) = O(n)`. ∎

### Why not groups of 3?

With groups of 3, the guaranteed partition is 1/3 vs 2/3:
```
T(n) = T(n/3) + T(2n/3) + O(n)
```
This gives `T(n) = O(n log n)` — no better than comparison sort.

With groups of 7 or more, the guarantee improves but the constant factor in the O(n) sorting-of-groups term grows. Groups of 5 is the sweet spot.

---

## Termination

- Each recursion operates on a strictly smaller range (the larger partition has at most 7n/10 elements, and the median-finding recursion has n/5 elements).
- Base case (n ≤ 5) is handled by insertion sort.
- Therefore the recursion terminates. ∎

---

## Partial ordering guarantee

After `select(first, nth, last)`:
- All elements in `[first, nth)` are ≤ `*nth`.
- All elements in `(nth, last)` are ≥ `*nth`.

This follows from the partition invariant, maintained through all recursive calls. ∎

---

## References

- M. Blum, R. W. Floyd, V. Pratt, R. L. Rivest, R. E. Tarjan, "Time bounds for selection," JCSS, 1973.
- T. H. Cormen et al., *Introduction to Algorithms (CLRS)*, Section 9.3: Selection in worst-case linear time.

