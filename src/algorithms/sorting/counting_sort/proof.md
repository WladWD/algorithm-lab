# Counting Sort — Proof of Correctness

## Algorithm sketch

Given an array `A[0..n)` of non-negative integers with maximum value `k-1`:
1. **Count:** Build a frequency array `count[0..k)` where `count[v]` = number of occurrences of value `v`.
2. **Prefix sum:** Compute cumulative sums so that `prefix[v]` = total number of elements with value ≤ `v`.
3. **Place:** Traverse the input in reverse; for each element `A[i]`, place it at position `--prefix[A[i]]` in the output.
4. Copy the output back.

---

## Correctness

### Claim: Every element is placed at a valid, unique position.

After computing prefix sums, `prefix[v]` equals the number of elements with value ≤ `v`. This means positions `prefix[v-1]..prefix[v]-1` (with `prefix[-1] = 0`) are reserved for elements with value `v`, and there are exactly `count[v]` such positions.

Each time we place an element with value `v`, we decrement `prefix[v]`. Since we place exactly `count[v]` elements of value `v`, `prefix[v]` decrements from its initial value down by `count[v]`, which exactly fills the reserved positions. No two elements are placed at the same index. ∎

### Claim: The result is sorted.

Elements with value `v` occupy positions `prefix[v-1]..prefix[v-1]+count[v]-1`, and elements with value `v+1` start at position `prefix[v]` ≥ `prefix[v-1]+count[v]`. Since values are assigned to contiguous, non-overlapping, increasing blocks of positions, the output is sorted. ∎

### Claim: The sort is stable.

We traverse the input array in **reverse** order. For elements with the same value `v`, the last one encountered (which was originally rightmost) gets the highest position (since `prefix[v]` is decremented before placement). Thus earlier occurrences end up at lower indices, preserving the original relative order. ∎

---

## Termination

- The counting loop iterates exactly `n` times.
- The prefix-sum computation iterates exactly `k` times.
- The placement loop iterates exactly `n` times.
- Total: O(n + k) operations, all bounded.

---

## Complexity

### Time: O(n + k)

Three linear passes: counting (O(n)), prefix sums (O(k)), and placement (O(n)).

### Space: O(n + k)

Frequency/prefix array of size `k`, plus an output array of size `n`.

