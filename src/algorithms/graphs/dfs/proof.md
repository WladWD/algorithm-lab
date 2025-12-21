# Depth-First Search (DFS) — Correctness Notes

This note matches the implementation in `src/dfs.cpp`.

We are given a graph `G = (V, E)` represented by an adjacency list `g` with vertices `V = {0, 1, ..., n-1}`.

The algorithm computes a **DFS forest**, consisting of one DFS tree per connected component (for undirected graphs) or per unreached region (for directed graphs).

---

## Outputs

The algorithm returns:

- `parent[v]`:
  - `-1` if `v` is a root of a DFS tree
  - otherwise the vertex from which `v` was first discovered
- `order`: the list of vertices in **discovery order** (preorder)
- `tin[v]`: time when `v` is first discovered
- `tout[v]`: time when the exploration of `v` finishes

All timestamps are produced by a single counter `timer` and are strictly increasing.

---

## Invariants

We maintain a color array:

- **white (0)**: not discovered
- **gray (1)**: discovered and currently on the DFS stack
- **black (2)**: fully processed

The explicit stack holds frames `(v, parent, next_idx)`.

> **Invariant A (stack correctness):**
> For every frame `(v, p, next_idx)` currently on the stack, `v` is gray, and the algorithm has already examined the first `next_idx` neighbors of `v`.

> **Invariant B (parent correctness):**
> When a vertex `v` transitions from white to gray, we set `parent[v] = p` where `p` is the vertex from which `v` was discovered, or `-1` if `v` is a DFS root.

> **Invariant C (timestamps):**
> `tin[v]` is assigned exactly once, at the moment `v` becomes gray.
> `tout[v]` is assigned exactly once, at the moment `v` becomes black.
> Moreover, for every vertex `v`, `tin[v] < tout[v]`.

---

## Termination

Each vertex changes color at most twice (white → gray → black).

For each gray vertex `v`, `next_idx` increases until it reaches `deg(v)`, at which point the frame is popped.

Since both the number of vertices and total neighbor iterations are finite, the algorithm terminates.

---

## Reachability and full coverage

Consider a DFS root `r` (a vertex chosen by the outer loop).

- `r` is pushed and becomes gray.
- Whenever the algorithm sees an edge `v -> to` with `to` white, it pushes a new frame for `to`.

This implies:

- Every vertex reachable from `r` will eventually be discovered (becomes gray), because the algorithm explores outgoing edges from every discovered vertex.
- No vertex is discovered more than once, because we only push a vertex when it is white.

As the outer loop starts a new DFS from every still-white vertex, **every vertex in `V` becomes visited**, and the result is a **DFS forest** spanning all vertices.

---

## Complexity

- Each vertex is pushed/popped at most once: `O(V)` stack operations.
- Each edge `v -> to` is inspected at most once when scanning adjacency of `v`: `O(E)`.

Total time is `O(V + E)`, and the extra memory is `O(V)`.

