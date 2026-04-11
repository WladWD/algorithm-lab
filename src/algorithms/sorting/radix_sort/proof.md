# Radix Sort — Proof of Correctness

## Algorithm sketch (LSD Radix Sort)

Given an array `A[0..n)` of non-negative integers, each with at most `d` digits in base `b`:

1. For digit position `j = 0, 1, ..., d-1` (least significant to most significant):
   - Stably sort `A` by digit `j` using counting sort.

---

## Key lemma: stability preserves earlier digit ordering

**Lemma:** If `A` is sorted by digits `0..j-1` (i.e., by the number formed by the lower `j` digits), then after stably sorting by digit `j`, `A` is sorted by digits `0..j`.

**Proof:** Consider two elements `x` and `y`.

- **Case 1: digit `j` of `x` < digit `j` of `y`.** After sorting by digit `j`, `x` precedes `y`. This is correct regardless of lower digits, because digit `j` is more significant.

- **Case 2: digit `j` of `x` > digit `j` of `y`.** After sorting by digit `j`, `y` precedes `x`. Correct for the same reason.

- **Case 3: digit `j` of `x` = digit `j` of `y`.** Since the sort is *stable*, `x` and `y` retain their relative order from the previous pass. By the inductive assumption, they were already in the correct order by digits `0..j-1`. Since digit `j` is the same, their full `(j+1)`-digit values are compared correctly. ∎

---

## Main theorem

**Claim:** After `d` passes of LSD radix sort, the array is fully sorted.

**Proof by induction on the number of passes:**

- **Base case (after pass 0):** The array is sorted by digit 0 (the least significant digit), since counting sort correctly sorts by a single digit.

- **Inductive step:** Assume after pass `j-1`, the array is sorted by the number formed by digits `0..j-1`. By the key lemma, after stably sorting by digit `j`, the array is sorted by digits `0..j`.

- **After pass `d-1`:** The array is sorted by all `d` digits, i.e., fully sorted. ∎

---

## Stability of the subroutine

The per-digit counting sort must be **stable**. Our implementation achieves this by:
1. Computing prefix sums of digit frequencies.
2. Traversing the input in **reverse** order, placing each element at `--prefix[digit]`.

This is the same stability mechanism proven in the counting sort proof.

---

## Signed integer support

For signed integers, we offset all values by subtracting the minimum:
`unsigned_val = val - min_val`

This maps the range `[min_val, max_val]` to `[0, max_val - min_val]`. After sorting the unsigned values, we reverse the offset. Since the mapping is monotone (order-preserving), sorting the unsigned values is equivalent to sorting the original signed values. ∎

---

## Termination

Each of the `d` passes iterates over `n` elements and `b` buckets. The number of passes is bounded by the number of bytes in the maximum value. All loops are bounded. ∎

---

## Complexity

### Time: O(d · (n + b))

Each pass is a counting sort taking O(n + b). With `d` passes: O(d · (n + b)).
For base-256 and 64-bit integers: d ≤ 8, b = 256, giving effectively O(n).

### Space: O(n + b)

One auxiliary array of size `n` plus a count array of size `b`, reused across passes.

