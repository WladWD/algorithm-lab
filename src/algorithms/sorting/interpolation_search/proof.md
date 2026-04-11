# Interpolation Search — Proof of Correctness

## Algorithm sketch

Given a sorted array `A[lo..hi]` of numeric values and a target `value`:

1. If `value < A[lo]` or `value > A[hi]`, the target is absent.
2. Compute the interpolated position:
   ```
   pos = lo + floor( (value - A[lo]) / (A[hi] - A[lo]) * (hi - lo) )
   ```
3. Compare `A[pos]` with `value`:
   - If `A[pos] < value`: the target lies in `A[pos+1..hi]`.
   - If `A[pos] > value`: the target lies in `A[lo..pos-1]`.
   - If `A[pos] == value`: found.
4. Repeat until found or the range is empty.

---

## Correctness of the interpolation formula

**Claim:** The interpolated position `pos` always falls within `[lo, hi]`.

**Proof:** Since `A[lo] <= value <= A[hi]` (checked before interpolation):
- The numerator `value - A[lo]` satisfies `0 <= value - A[lo] <= A[hi] - A[lo]`.
- The ratio `(value - A[lo]) / (A[hi] - A[lo])` is in `[0, 1]`.
- Multiplying by `(hi - lo)` gives an offset in `[0, hi - lo]`.
- Therefore `pos = lo + offset` is in `[lo, hi]`. ∎

In the implementation, we additionally clamp the offset to guard against floating-point rounding.

---

## Loop invariant

At the start of each iteration:
- `lo <= hi` (the search range is non-empty).
- `A[lo] <= value <= A[hi]` (the target, if present, lies within the range).
- All elements outside `[lo, hi]` have been ruled out.

### Initialization

Before the first iteration, `lo = first` and `hi = last - 1`. The range check `value >= A[lo] && value <= A[hi]` in the loop condition establishes the invariant.

### Maintenance

After probing at position `pos`:
- If `A[pos] < value`: we set `lo = pos + 1`. Since `A` is sorted and `A[pos] < value`, all elements in `A[lo_old..pos]` are `< value`. The new range `[pos+1, hi]` still satisfies the invariant.
- If `A[pos] > value`: we set `hi = pos - 1`. By the same argument, elements `A[pos..hi_old]` are `> value`.
- If `A[pos] == value`: we return immediately.

### Termination

**Case 1 (value found):** The algorithm returns the position.

**Case 2 (range exhausted):** When `lo > hi`, the range is empty and the value is absent.

**Case 3 (value out of range):** When `value < A[lo]` or `value > A[hi]`, the loop condition fails and we return "not found".

In all cases, the search range strictly shrinks (by at least 1 element) each iteration, so the loop terminates in at most `hi - lo + 1` iterations.

---

## Correctness of the search

**Claim:** `interpolation_search(A, value)` returns an iterator to an element equal to `value` if one exists, or `last` otherwise.

**Proof:** By the loop invariant, every eliminated element is provably not equal to `value` (since the array is sorted and the element is on the wrong side of the probe). If the element exists, it remains in the search range until found. If it does not exist, the range eventually becomes empty and we return `last`. ∎

---

## Correctness of lower_bound variant

**Claim:** `interpolation_lower_bound(A, value)` returns an iterator to the first element `>= value`.

**Proof:** The implementation follows the same structure as binary search lower_bound, but uses the interpolation formula to choose the probe position instead of the midpoint. The key properties are:

1. If `*mid < value`, then all elements in `[lo, mid]` are `< value`, so `lo = mid + 1`.
2. If `*mid >= value`, then the answer could be at `mid` or before, so `hi = mid`.
3. The range `[lo, hi)` shrinks each iteration (since the probe is always within range).
4. When `lo == hi`, it points to the first element `>= value`.

This is the same invariant as binary search lower_bound, just with a different probe strategy. ∎

---

## Expected time complexity: O(log log n)

**Theorem (Perl, Itai, Avni, 1978):** On uniformly distributed data, interpolation search makes O(log log n) expected comparisons.

**Intuition:** If the data is uniformly distributed in `[A[lo], A[hi]]`, the interpolation formula predicts the target's position with error roughly `O(√(hi - lo))`. This means each probe reduces the range from `n` to approximately `√n`, giving the recurrence:

```
T(n) = T(√n) + O(1)
```

Substituting `n = 2^k`: `T(2^k) = T(2^(k/2)) + O(1)`, which gives `T(n) = O(log k) = O(log log n)`.

---

## Worst-case time complexity: O(n)

**Example:** If all elements except one are clustered at one end (e.g., `[1, 1, 1, ..., 1, 1000000]` searching for `999999`), the interpolation formula consistently overestimates, and each probe only eliminates one element. This gives:

```
T(n) = T(n-1) + O(1) = O(n)
```

This is why interpolation search should only be used when the distribution is expected to be roughly uniform.

---

## References

- Y. Perl, A. Itai, H. Avni, "Interpolation search — a log log n search," Communications of the ACM, 1978.
- T. H. Cormen et al., *Introduction to Algorithms (CLRS)*, Problem 2-3 (analysis of interpolation search).
- A. C. Yao, F. F. Yao, "The complexity of searching an ordered random table," FOCS 1976.

