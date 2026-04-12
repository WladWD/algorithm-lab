# TimSort — Proof of Correctness

## Algorithm sketch

1. **Identify natural runs:** Scan the array for maximal ascending or strictly descending runs. Reverse descending runs in-place.
2. **Extend short runs:** If a run is shorter than `min_run` (≈32–64), extend it using insertion sort.
3. **Push runs onto a stack** and merge adjacent runs to maintain the merge invariant.
4. **Final collapse:** Merge all remaining runs on the stack until only one remains.

---

## Correctness of run detection

**Claim:** After run detection (and reversal of descending runs), each identified run is a sorted sub-array.

- Ascending runs (`A[i] <= A[i+1] <= ...`) are already sorted.
- Strictly descending runs (`A[i] > A[i+1] > ...`) become sorted after in-place reversal. Using strict descent ensures stability: equal elements are never reversed.

∎

---

## Correctness of insertion sort extension

Short natural runs are extended to `min_run` length using insertion sort. Since insertion sort correctly sorts any sub-array, the extended run is sorted. ∎

---

## Merge correctness

The merge operation takes two adjacent sorted runs and produces one sorted run. This follows the standard merge proof (see MergeSort proof.md): elements are picked in order from two sorted sequences, with ties broken in favor of the left run (stability).

**Stability:** On equal elements, we pick from the left run. Since runs maintain their original left-to-right order, equal elements across all runs preserve their original relative positions. ∎

---

## Merge stack invariant

TimSort maintains a stack of pending runs. After pushing each new run, it enforces:
1. `stack[i-2].length > stack[i-1].length + stack[i].length`
2. `stack[i-1].length > stack[i].length`

for the top three entries.

### Why this invariant matters

This guarantees that run lengths on the stack grow at least as fast as the Fibonacci sequence. Since Fibonacci numbers grow exponentially, the stack depth is bounded by O(log n). This ensures:
- The total merge work is O(n log n).
- The stack never exceeds ~log₂(n) entries.

### Enforcement

When the invariant is violated, adjacent runs are merged (always merging the shorter pair to minimize work). This is repeated until the invariant holds.

---

## Overall correctness

**Claim:** After TimSort completes, the array is a sorted permutation of the input.

**Proof:**
1. The array is partitioned into non-overlapping, sorted runs (by run detection + extension).
2. Adjacent runs are merged pairwise (preserving sortedness and stability).
3. The final collapse merges all remaining runs into one.
4. Since merge is correct and covers the entire array, the result is sorted. ∎

---

## Stability

At every stage:
- Run detection preserves original order (strict descent avoids reversing equal elements).
- Insertion sort is stable.
- Merging is stable (left-side preference on ties).
- Runs are merged left-to-right (preserving global order of equal elements across runs).

Therefore TimSort is stable. ∎

---

## Complexity

### Best case: O(n)

If the array is already sorted, TimSort detects a single run of length n and does no merging. The run detection pass is O(n).

### Worst case: O(n log n)

The merge stack invariant ensures run lengths grow at least like Fibonacci numbers. With n elements, the stack has at most O(log n) entries. Each merge level processes all n elements once. Total: O(n log n).

More precisely: the total number of merge operations is bounded by the total "weight" of the merge tree. Since the invariant enforces near-Fibonacci growth, the merge tree has O(log n) levels, each doing O(n) work.

### Space: O(n)

The merge buffer is at most half the array size (we always copy the smaller side). In the worst case, this is O(n/2) = O(n).

---

## References

- Tim Peters, "listsort.txt" — original design document (CPython source, 2002).
- "Formal verification of TimSort" — de Gouw et al., CAV 2015 (correctness of the merge invariant).
- OpenJDK `java.util.TimSort` — Java implementation reference.

