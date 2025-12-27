# Disjoint Set Union (Union-Find)  Correctness Notes

This note matches the implementation in `src/dsu.cpp`.

We store two arrays:

- `parent[v]`  a pointer to some vertex in the same set
- `size[v]`  valid only when `v` is a root; equals the number of vertices in the roots set

A vertex `r` is a **root** iff `parent[r] = r`.
Each component is represented by exactly one root.

---

## Invariant

For all vertices `v` in range:

1. Following `parent` pointers from `v` eventually reaches a root `r`.
2. Vertices that reach the same root belong to the same component.
3. For any root `r`, `size[r]` equals the number of vertices whose root is `r`.

Initially (after `assign(n)`), every vertex is a root and every set has size 1, so the invariant holds.

---

## Correctness of `find`

`find(v)` returns the root reached by following parent pointers.

- Because union operations only ever attach a root under another root, the parent pointers always
  form a forest of rooted trees (no cycles).
- So the loop that walks `parent` pointers must reach a root and return a valid representative.

### Path compression

After the root `r` is found, the implementation rewires all visited vertices to point directly to `r`.
This **does not change which vertices are connected**; it only shortens paths in the tree.
Therefore it preserves the invariant.

---

## Correctness of `unite`

Let roots be `ra = find(a)` and `rb = find(b)`.

- If `ra == rb`, then `a` and `b` are already in the same component, so no merge is needed.
- Otherwise, the algorithm attaches one root under the other:

  `parent[rb] := ra` (after possibly swapping so that `size[ra] >= size[rb]`).

After this assignment:

- Every vertex that previously reached `rb` now reaches `ra` (possibly through `rb`), so the two sets
  are merged into a single component.
- No other components are affected.
- The number of components decreases by exactly 1.

### Correctness of `size`

Before merge, the sets are disjoint, so the new component size is:

`|S(ra) ∪ S(rb)| = |S(ra)| + |S(rb)|`.

The code sets:

`size[ra] := size[ra] + size[rb]`.

Thus the size invariant remains true for the new root `ra`.

---

## Union by rank variant

In the rank-based variant, we store an additional `rank` array.

- `rank[v]`  valid only when `v` is a root; an upper bound on the height of the tree rooted at `v`

Merging attaches the smaller tree under the root of the larger tree, preserving the invariant that
smaller trees are always attached under larger trees.

Component sizes are still maintained exactly by storing `size[root]` and adding them on merge, same as in the size-based variant.

### Time complexity note

With union by rank + path compression, a sequence of `m` operations on `n` elements runs in `O(m α(n))` time (Tarjan),
where `α` is the inverse Ackermann function (<= 5 for any practical `n`).
