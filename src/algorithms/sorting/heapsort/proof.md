# HeapSort — Proof of Correctness

## Algorithm sketch

1. **Build a max-heap** from the array (bottom-up, in-place).
2. **Extract-max loop:** swap the root (maximum) with the last unsorted element, shrink the heap by one, and sift-down to restore the heap property.

---

## Heap property

A **max-heap** stored in an array satisfies: for every index `i`, `A[i] >= A[2i+1]` and `A[i] >= A[2i+2]` (where children exist). Equivalently, every node's value is ≥ its children's values.

---

## Sift-down correctness

**Claim:** `sift_down(A, n, i)` restores the max-heap property for the subtree rooted at `i`, assuming both children's subtrees are already valid max-heaps.

**Proof (by induction on the height of node `i`):**

- **Base case (leaf):** A leaf has no children, so the heap property holds trivially.
- **Inductive step:** Compare `A[i]` with its children. If `A[i]` is already the largest, done. Otherwise, swap `A[i]` with the largest child `A[c]`. Now:
  - The subtree rooted at `i` satisfies the heap property at the root (since we placed the largest value there).
  - The subtree rooted at `c` may be violated only at `c` itself (the old `A[i]` was placed there), but `c`'s children's subtrees are still valid heaps.
  - By the inductive hypothesis, continuing sift-down from `c` restores the subtree. ∎

---

## Build-heap correctness

**Claim:** The bottom-up build loop `for i = n/2 - 1 down to 0: sift_down(A, n, i)` produces a valid max-heap.

**Proof:** Process nodes from the last internal node up to the root. When `sift_down(A, n, i)` is called, all nodes with index > `i` already satisfy the heap property (they were processed earlier or are leaves). By the sift-down correctness claim, after the call, the subtree rooted at `i` is a valid max-heap.

After processing `i = 0`, the entire array is a valid max-heap. ∎

---

## Extract-max loop correctness

**Loop invariant:** After `k` iterations of the extract loop:
- `A[n-k..n)` contains the `k` largest elements in sorted (ascending) order.
- `A[0..n-k)` is a valid max-heap containing the remaining elements.

**Initialization (k = 0):** The entire array is a max-heap; the sorted suffix is empty.

**Maintenance:** The root `A[0]` is the maximum of the heap `A[0..n-k)`. Swapping it with `A[n-k-1]` places it in the correct final position. After the swap, `A[0..n-k-1)` may violate the heap property only at the root. `sift_down(A, n-k-1, 0)` restores it.

**Termination (k = n-1):** The sorted suffix is `A[1..n)` and `A[0]` is the single remaining element (trivially the minimum). The entire array is sorted. ∎

---

## Termination

- `sift_down` follows a path from node `i` to a leaf. Tree height is `O(log n)`, so it terminates in at most `O(log n)` steps.
- The build loop iterates exactly `n/2` times.
- The extract loop iterates exactly `n - 1` times.

---

## Complexity

### Time: O(n log n)

- **Build phase:** O(n) — each sift-down takes O(height), and summing over all nodes gives the well-known O(n) result (most nodes are near the bottom).
- **Extract phase:** `n - 1` extract-max operations, each requiring one sift-down of O(log n).
- Total: O(n) + O(n log n) = **O(n log n)**.

### Space: O(1)

HeapSort is fully in-place. Only a constant number of local variables are used beyond the input array.

