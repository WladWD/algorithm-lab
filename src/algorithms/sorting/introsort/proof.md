# IntroSort — Proof of Correctness

## Algorithm sketch

1. Begin with QuickSort (median-of-three pivot, Hoare partition).
2. Track recursion depth. If it exceeds `2·⌊log₂ n⌋`, switch to HeapSort on the current sub-range.
3. Sub-ranges of ≤16 elements are left unsorted during partitioning.
4. A final insertion sort pass cleans up the nearly-sorted array.

---

## Correctness of individual components

### QuickSort partition

The Hoare partition scheme partitions `[first, last)` around a pivot such that:
- All elements in `[first, lo)` are ≤ pivot.
- All elements in `[lo, last)` are ≥ pivot.

After placing the pivot at `lo`, elements left of `lo` are ≤ pivot and elements right are ≥ pivot. Recursively sorting both halves yields a fully sorted result. (See QuickSort proof.md for the detailed partition invariant.)

### HeapSort fallback

HeapSort correctly sorts any sub-range in O(n log n) worst-case time. (See HeapSort proof.md.)

### Insertion sort finish

After the partitioning phase, every element is within 16 positions of its final sorted position (because partitions of size ≤16 were left unsorted). Insertion sort on a k-displaced array runs in O(n·k) time. With k ≤ 16, this is O(16n) = O(n).

---

## Overall correctness

**Claim:** IntroSort produces a sorted permutation of the input.

**Proof:** The algorithm partitions the array recursively. At each level:
- If depth limit is not reached: QuickSort partitions the sub-range into two parts, each sorted recursively (by induction).
- If depth limit is reached: HeapSort sorts the sub-range directly.
- Sub-ranges of ≤16 elements are left for the final pass.

After all recursive/heapsort calls, every element is at most 16 positions from its final position. The final insertion sort completes the sorting. By correctness of QuickSort partition + HeapSort + insertion sort, the result is a sorted permutation. ∎

---

## Worst-case O(n log n) guarantee

**Claim:** IntroSort runs in O(n log n) time in the worst case.

**Proof:**

The depth limit is set to `2·⌊log₂ n⌋`. Consider the recursion tree:

**Case 1: HeapSort never triggers.** All partitioning stays within the depth limit. With tail-recursion elimination (always recursing on the smaller partition), the depth is at most log₂(n). Each level does O(n) total work. Total: O(n log n).

**Case 2: HeapSort triggers on some sub-ranges.** When depth exceeds the limit, the remaining sub-range of size `m` is sorted by HeapSort in O(m log m). The total partitioning work before HeapSort triggers is bounded by `O(n · depth_limit) = O(n log n)` (at most `2 log₂(n)` levels, each doing O(n) work). The HeapSort work across all fallback calls is at most O(n log n) total (since the sub-ranges are disjoint and sum to ≤ n).

**Final insertion sort:** O(n) as shown above.

Total: O(n log n) + O(n log n) + O(n) = **O(n log n)**. ∎

---

## Termination

- QuickSort partitioning produces strictly smaller sub-ranges each step.
- HeapSort is guaranteed to terminate (finite loop).
- Insertion sort terminates (finite array).
- Depth limit is finite, preventing unbounded recursion. ∎

---

## References

- D. R. Musser, "Introspective Sorting and Selection Algorithms," Software: Practice & Experience, 1997.
- Most `std::sort` implementations (libstdc++, libc++, MSVC) follow this pattern.

