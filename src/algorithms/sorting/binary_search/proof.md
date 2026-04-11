# Binary Search — Proof of Correctness

## lower_bound

### Specification

Given a sorted range `[first, last)` and a value `v`, return the leftmost iterator `it` such that `!comp(*it, v)` (i.e., `*it >= v`). If no such element exists, return `last`.

### Loop invariant

At the start of each iteration:
- All elements in `[original_first, first)` satisfy `comp(*x, v)` (i.e., `*x < v`).
- All elements in `[last, original_last)` satisfy `!comp(*x, v)` (i.e., `*x >= v`).
- The answer lies in `[first, last]`.

### Proof

**Initialization:** `first = original_first`, `last = original_last`. The invariant holds vacuously (both prefix and suffix ranges are empty).

**Maintenance:** Let `mid = first + (last - first) / 2`.
- If `comp(*mid, v)`: `*mid < v`, so `mid` belongs in the "too small" prefix. Set `first = mid + 1`. The invariant is maintained because we've extended the prefix.
- Otherwise: `*mid >= v`, so `mid` could be the answer or there's something earlier. Set `last = mid`. The invariant is maintained because we've extended the suffix.

**Termination:** The range `[first, last)` shrinks by at least 1 each iteration (`mid` is strictly between `first` and `last` when the range has ≥2 elements, and the loop exits when `first >= last`). When `first == last`, it points to the boundary between "too small" and "≥ v" — exactly the lower bound. ∎

---

## upper_bound

### Specification

Return the leftmost iterator `it` such that `comp(v, *it)` (i.e., `*it > v`).

### Loop invariant

At the start of each iteration:
- All elements in `[original_first, first)` satisfy `!comp(v, *x)` (i.e., `*x <= v`).
- All elements in `[last, original_last)` satisfy `comp(v, *x)` (i.e., `*x > v`).

### Proof

Identical structure to `lower_bound`, but with the comparison reversed:
- If `comp(v, *mid)`: `*mid > v`, set `last = mid`.
- Otherwise: `*mid <= v`, set `first = mid + 1`.

Termination: same argument. When `first == last`, it points to the first element strictly greater than `v`. ∎

---

## binary_search (existence)

### Specification

Return `true` if `v` exists in the sorted range.

### Proof

We compute `it = lower_bound(first, last, v, comp)`. By the lower_bound specification:
- `it` points to the first element `>= v`.
- If `it != last` and `!comp(v, *it)`, then `*it` is neither less than `v` (by lower_bound) nor greater than `v` (by the second check). Under strict weak ordering, this means `*it` is equivalent to `v`. ∎

---

## equal_range

### Specification

Return `{lower_bound(...), upper_bound(...)}`, defining the maximal subrange of elements equivalent to `v`.

### Proof

Follows directly from the correctness of `lower_bound` and `upper_bound`. The range `[lb, ub)` contains exactly those elements `x` for which `!comp(x, v) && !comp(v, x)` (i.e., equivalent to `v`). ∎

---

## Termination

Each iteration halves the search range (±1). Starting from size `n`, the range reaches 0 after at most `⌊log₂ n⌋ + 1` iterations.

---

## Complexity

### Time: O(log n)

Each iteration reduces the range by at least half. Total comparisons: ≤ `⌊log₂ n⌋ + 1`.

### Space: O(1)

Only a constant number of iterator variables.

