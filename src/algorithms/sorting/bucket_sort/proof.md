# Bucket Sort — Proof of Correctness

## Algorithm sketch

Given an array `A[0..n)` of numeric values:
1. Find `min_val` and `max_val`.
2. Create `k` empty buckets, each covering an equal sub-range of `[min_val, max_val]`.
3. Distribute each element into its bucket: `bucket_idx = ⌊(A[i] - min_val) / range * k⌋`.
4. Sort each bucket individually (using insertion sort).
5. Concatenate all buckets in order.

---

## Correctness

### Claim: The final array is a sorted permutation of the input.

**Proof:**

**Permutation property:** Every element is placed into exactly one bucket (step 3) and every bucket element is copied back (step 5). No elements are created or destroyed. ∎

**Sorted property:** Consider any two elements `x` and `y` with `x < y` in the final output.

- **Case 1: `x` and `y` are in different buckets.** Since `x < y`, the bucket index of `x` is ≤ the bucket index of `y` (the mapping from value to bucket is monotone). Buckets are concatenated in order, so `x` appears before `y`. ∎

- **Case 2: `x` and `y` are in the same bucket.** The within-bucket sort (insertion sort) places `x` before `y`. ∎

### Stability

Within each bucket, insertion sort is stable (elements maintain their original relative order). Elements are distributed to buckets in their original order, so for equal-valued elements in the same bucket, relative order is preserved. ∎

---

## Distribution correctness

**Claim:** The bucket index formula `⌊(A[i] - min_val) / range * k⌋` maps each element to a bucket in `[0, k)`.

**Proof:** Since `min_val ≤ A[i] ≤ max_val`:
- `A[i] - min_val` is in `[0, max_val - min_val]`.
- Dividing by `range` (slightly enlarged to handle the max endpoint) gives a value in `[0, 1)`.
- Multiplying by `k` and flooring gives an index in `[0, k-1]`. ∎

---

## Complexity

### Expected time: O(n + k)

If elements are uniformly distributed, each bucket receives O(n/k) elements in expectation. Insertion sort on a bucket of size `m` takes O(m²). The total expected work is:

```
E[total] = Σ E[m_i²] for i = 0..k-1
```

For uniform distribution with k = n buckets, E[m_i] = 1, and E[m_i²] = O(1). Total: O(n).

### Worst case: O(n²)

If all elements fall into one bucket, insertion sort on that bucket takes O(n²).

### Space: O(n + k)

`k` bucket vectors plus `n` total elements stored across them.

---

## References

- T. H. Cormen et al., *Introduction to Algorithms (CLRS)*, Chapter 8: Sorting in Linear Time.
- Bucket sort analysis assumes uniform distribution for O(n) expected time.

