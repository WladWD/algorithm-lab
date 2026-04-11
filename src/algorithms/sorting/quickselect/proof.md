# QuickSelect — Proof of Correctness

## Algorithm sketch

Given a range `[first, last)` and a target position `nth`:
1. Partition the range into three regions: `[first, lt)` < pivot, `[lt, gt)` == pivot, `[gt, last)` > pivot.
2. If `nth < lt`: recurse (loop) on `[first, lt)`.
3. If `nth >= gt`: recurse (loop) on `[gt, last)`.
4. If `lt <= nth < gt`: the element at `nth` is the pivot value — done.

---

## Partition correctness

The three-way partition maintains the invariant (same as QuickSort):
- `[first, lt)`: all elements < pivot
- `[lt, i)`: all elements == pivot
- `[i, gt)`: unclassified
- `[gt, last)`: all elements > pivot

When `i == gt`, the entire range is classified. See the QuickSort proof for details.

---

## Selection correctness

**Claim:** After `quickselect(first, nth, last)`, the element at position `nth` is the element that would be at position `nth` in the fully sorted range.

**Proof by induction on range size:**

**Base case (size ≤ 1):** A single element is trivially in the correct position.

**Inductive step:** After partitioning:
- Every element in `[first, lt)` is less than the pivot.
- Every element in `[lt, gt)` equals the pivot.
- Every element in `[gt, last)` is greater than the pivot.

**Case 1: `nth` falls in `[lt, gt)`.** The position `nth` contains the pivot value. In a fully sorted array, positions `lt` through `gt - 1` would also contain the pivot value (since there are exactly `lt - first` elements smaller and `last - gt` elements larger). So the element at `nth` is correct. ∎

**Case 2: `nth < lt`.** The target position is among the "less than pivot" elements. We recurse on `[first, lt)`. By the inductive hypothesis (range is strictly smaller), the element at `nth` within this subrange is correctly placed. Since all elements in `[lt, last)` are ≥ pivot and all elements in `[first, lt)` are < pivot, the global position is also correct. ∎

**Case 3: `nth >= gt`.** Symmetric to Case 2, recursing on `[gt, last)`. ∎

---

## Partial ordering guarantee

After quickselect:
- All elements in `[first, nth)` are ≤ `*nth`.
- All elements in `(nth, last)` are ≥ `*nth`.

This follows from the partition invariant: elements to the left of the pivot region are smaller, and elements to the right are larger. Each recursive narrowing preserves this global invariant.

---

## Termination

Each partition produces at least one element in the "== pivot" region (the pivot itself). Therefore:
- If `nth` is in the pivot region, we stop immediately.
- Otherwise, the range strictly shrinks (at least the pivot is excluded from the recursive range).

The range size decreases by at least 1 per iteration, so the loop terminates.

---

## Complexity

### Expected time: O(n)

With a good pivot (e.g., median-of-three), each partition is expected to eliminate a constant fraction of elements. The recurrence is:

```
T(n) = T(αn) + O(n)    for some α < 1
```

By the geometric series: T(n) = O(n / (1 - α)) = O(n).

### Worst-case time: O(n²)

If every partition is maximally unbalanced (pivot is always the extremum), the range shrinks by only 1 each step: T(n) = T(n-1) + O(n) = O(n²).

### Space: O(1)

The implementation is iterative (tail-call eliminated), using only a constant number of variables.

