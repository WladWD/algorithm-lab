# QuickSort — Proof of Correctness

## Algorithm sketch

Given an array segment `A[l..r)`:
1. Choose a pivot value `p`.
2. Partition the segment into three consecutive regions:
   - values `< p`
   - values `== p`
   - values `> p`
3. Recursively sort the `< p` region and the `> p` region.

The implementation in this repository uses **three-way partitioning** (Dutch National Flag), a median-of-three pivot, and a depth guard (fallback to `std::sort`) to avoid pathological recursion.

---

## Partition correctness

During partitioning, the algorithm maintains the invariant:

- `A[first..lt) < p`
- `A[lt..i) == p`
- `A[i..gt) = unknown`
- `A[gt..last) > p`

Initially:
- `lt = first`, `i = first`, `gt = last`, so the invariant holds trivially.

At each step, we inspect `A[i]`:
- If `A[i] < p`, swap it into the `< p` region (swap with `A[lt]`) and advance both `lt` and `i`.
- If `A[i] > p`, swap it into the `> p` region (swap with `A[gt-1]`) and decrement `gt`.
  We do *not* increment `i` because the element swapped in from the right is unclassified.
- Otherwise (`A[i] == p`), just increment `i`.

Each operation preserves the invariant and strictly shrinks the unknown region `[i..gt)`, so the loop terminates. When it finishes, the entire segment is partitioned into the three desired regions.

---

## Sorting correctness

After partitioning we have:

- all elements in the left region are `< p`
- all elements in the middle region equal `p`
- all elements in the right region are `> p`

If we recursively sort the left and right regions, then:
- every element of the left region is `<=` every element in the middle region
- every element of the middle region is `<=` every element in the right region

Thus the whole segment is sorted.

This argument can be formalized by induction on segment length:
- Base case: segments of size `0` or `1` are already sorted.
- Induction step: assume correctness for smaller segments; partitioning produces two strictly smaller subproblems (left and right), which by the induction hypothesis are sorted after recursion; the pivot-based ordering between regions implies the concatenation is sorted.

---

## Termination

Each partition step produces subranges that are strictly smaller than the input range unless all elements are equal (in which case both recursive calls become empty). Therefore recursion terminates.

In practice, this implementation also:
- uses insertion sort for tiny ranges
- uses a depth guard to avoid unbounded recursion depth on adversarial inputs

---

## Depth guard (why it avoids sustained `O(n^2)` behavior)

Classic QuickSort can degrade to **quadratic time** when the partition is repeatedly extremely unbalanced.
A common worst-case pattern is a pivot that always splits as `0` and `n-1` (or similarly skewed), giving the recurrence:

```
T(n) = T(n-1) + O(n)  =>  T(n) = O(n^2)
```

This is also reflected in the recursion depth: a sequence of such splits produces recursion depth `O(n)`.

### What the guard does

This implementation tracks a recursion limit of approximately:

- `depth_limit ≈ 2 * floor(log2(n)) + 1`

If recursion exceeds this limit, the algorithm **stops using QuickSort** on the current subrange and falls back to a robust `O(m log m)` comparison sort (`std::sort`) for that subrange of size `m`.

### Why this prevents quadratic time from persisting

Intuition: to get `O(n^2)` total work, QuickSort must keep producing very unbalanced splits *for many levels*.
The depth guard prevents that from continuing beyond `O(log n)` levels.

More concretely:

1. **Before the guard triggers**, the algorithm performs at most `depth_limit` partitioning levels along any path.
   Each partitioning level costs linear work in the size of the current subrange.
2. If partitions are reasonably balanced, recursion depth stays around `O(log n)` anyway, and the guard never triggers.
3. If partitions are persistently unbalanced (the scenario that leads to `O(n^2)`), recursion depth grows quickly.
   Once it exceeds the limit, the remaining sorting work on the current subrange is delegated to `std::sort`, which is designed to have `O(m log m)` worst-case time.

A useful way to view this is “**QuickSort until it looks bad**”. The guard cuts off the bad case early.

### Relationship to Introsort

The idea matches **introsort**: start with QuickSort for speed, but use a depth-based trigger to switch to a worst-case `O(n log n)` method.
Most standard library implementations of `std::sort` are introsort-like, so falling back to `std::sort` gives the intended robustness.
