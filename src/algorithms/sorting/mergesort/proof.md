# MergeSort — Proof of Correctness

## Algorithm sketch

Given an array segment `A[l..r)`:
1. If the segment has ≤1 element, it is trivially sorted.
2. Split at the midpoint: `mid = l + (r - l) / 2`.
3. Recursively sort `A[l..mid)` and `A[mid..r)`.
4. Merge the two sorted halves into a single sorted segment.

---

## Merge correctness

**Claim:** Given two sorted sub-arrays `A[l..mid)` and `A[mid..r)`, the merge procedure produces a sorted `A[l..r)` containing all original elements.

**Proof:**

The merge maintains two pointers (`left` and `right`) starting at the beginning of each half. At each step, the smaller of the two pointed-to elements is appended to the output buffer.

**Loop invariant:** At the start of each iteration, the output buffer contains a sorted sequence of elements, all of which are ≤ both `*left` and `*right`.

- **Initialization:** The buffer is empty; the invariant holds vacuously.
- **Maintenance:** We pick `min(*left, *right)` and append it. Since both halves are sorted, this element is ≥ all previously picked elements. On ties we pick from the left half (preserving stability).
- **Termination:** When one pointer is exhausted, the remaining elements of the other half are already sorted and ≥ everything in the buffer. Appending them preserves sortedness.

After copying the buffer back, `A[l..r)` is sorted and contains exactly the original multiset of elements. ∎

---

## Sorting correctness (by strong induction)

**Claim:** `mergesort(A[l..r))` produces a sorted permutation of `A[l..r)`.

**Proof by strong induction on `n = r - l`:**

- **Base case (n ≤ 1):** A segment of 0 or 1 elements is already sorted.
- **Inductive step:** Assume correctness for all segments of size `< n`. Since `mid - l < n` and `r - mid < n`, both recursive calls produce sorted halves (by the inductive hypothesis). By the merge correctness claim above, the combined result is a sorted permutation of the original segment. ∎

---

## Stability

**Claim:** MergeSort is stable — elements comparing equal retain their original relative order.

**Proof:** During the merge step, when `*left` and `*right` compare equal (i.e., `!comp(*right, *left)` is true), we pick from the left half. Since the left half precedes the right half in the original order, equal elements from the left appear before equal elements from the right in the output. By induction, each recursive half is itself stably sorted, so the global relative order of equal elements is preserved. ∎

---

## Termination

Each recursive call operates on a strictly smaller segment (both halves have size `< n`). The base case is reached when `n ≤ 1`. Therefore the recursion terminates.

---

## Complexity

### Time: O(n log n)

The recurrence is `T(n) = 2T(n/2) + O(n)` (two half-size subproblems + a linear merge). By the Master Theorem (case 2), `T(n) = O(n log n)`. This holds in the best, average, and worst case, since the split is always at the midpoint.

### Space: O(n)

The implementation uses a single auxiliary buffer of size `n`, shared across all recursive calls. Recursion depth is `O(log n)` (stack space), dominated by the `O(n)` buffer.

