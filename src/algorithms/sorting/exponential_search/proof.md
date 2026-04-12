# Exponential Search — Proof of Correctness

## Algorithm sketch

Given a sorted array `A[0..n)` and target `value`:

**Phase 1 (Exponential expansion):**
1. Start with `bound = 1`.
2. While `bound < n` and `A[bound] < value`: double `bound`.
3. The target (if present) lies in `[bound/2, min(bound+1, n))`.

**Phase 2 (Binary search):**
4. Binary-search for `value` within the bounded range.

---

## Phase 1 correctness

**Claim:** After the expansion loop, if `value` exists in the array, it lies in `[bound/2, min(bound+1, n))`.

**Proof:**

Let `bound_prev = bound / 2` (the value of `bound` before the last doubling).

- The loop condition `A[bound] < value` was true for `bound_prev`, so `A[bound_prev] < value`. This means all elements at indices `< bound_prev` are also `< value` (array is sorted), so the target cannot be at index `< bound_prev`.
- The loop exited because either `bound >= n` or `A[bound] >= value`. In either case, the target (if it exists) is at index `< min(bound + 1, n)`.
- Therefore the target is in `[bound/2, min(bound + 1, n))`. ∎

**Special case:** If `A[0] >= value`, the loop never executes and `bound = 1`, giving the search range `[0, 1)` or `[0, min(2, n))`, which correctly includes position 0.

---

## Phase 2 correctness

Standard binary search on the bounded range. See the Binary Search proof for the invariant and termination arguments. ∎

---

## Overall correctness

**Claim:** `exponential_search(A, value)` returns an iterator to `value` if present, or `last` otherwise.

**Proof:** Phase 1 identifies a range of size at most `2 * i` (where `i` is the target's index) that contains the target if it exists. Phase 2 is a correct binary search on that range. ∎

---

## Lower-bound variant

`exponential_lower_bound` uses the same Phase 1, then performs a binary-search lower_bound on the bounded range. By the correctness of Phase 1, the first element `>= value` lies within the bounded range (or is at `last`). The binary-search lower_bound correctly finds it. ∎

---

## Termination

**Phase 1:** `bound` doubles each iteration. After at most `⌈log₂ n⌉` iterations, `bound >= n` and the loop exits.

**Phase 2:** Standard binary search terminates in `O(log(bound))` iterations.

---

## Complexity

### Time: O(log i)

Phase 1 performs `⌈log₂ i⌉` iterations (doublings until `bound >= i`). Phase 2 searches a range of size `≤ bound ≤ 2i`, taking `O(log(2i)) = O(log i)` comparisons.

Total: `O(log i) + O(log i) = O(log i)`, where `i` is the index of the target.

**Worst case:** If the target is at the end, `i = n - 1`, so the time is `O(log n)` — same as binary search.

### Space: O(1)

Only a constant number of variables (bound, lo, hi, mid).

---

## References

- J. L. Bentley and A. C. Yao, "An almost optimal algorithm for unbounded searching," Information Processing Letters, 1976.
- Exponential search is the basis of *galloping mode* in TimSort's merge step.

