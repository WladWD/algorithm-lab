# Breadth-First Search (BFS) — Correctness Notes

This note matches the implementation in `src/bfs.cpp`.

We are given a graph `G = (V, E)` represented by an adjacency list `g` with vertices `V = {0, 1, ..., n-1}`.

BFS explores vertices in **non-decreasing distance** from a chosen start vertex (or from the chosen component root for the forest version).

---

## Outputs

### `bfs_from(g, start)` returns `BfsResult`

- `dist[v]`:
  - `-1` if `v` is unreachable from `start`
  - otherwise the length (number of edges) of a shortest path from `start` to `v`
- `parent[v]`:
  - `-1` for `start` and for unreachable vertices
  - otherwise a predecessor of `v` on a shortest path
- `order`: vertices in the order they are first discovered (enqueued)

### `bfs_forest(g)` returns `BfsForest`

Same fields, but distances are measured from the **component root** selected by the outer loop.

---

## Invariants

The algorithm maintains:

- an array `dist` initialized to `-1` (meaning "unreached")
- a FIFO queue `q` of discovered vertices whose outgoing edges are not fully scanned yet

> **Invariant A (discovery + parent assignment):**
> When a vertex `to` is first discovered from a vertex `v`, the algorithm sets:
>
> - `dist[to] = dist[v] + 1`
> - `parent[to] = v`
>
> and enqueues `to`.

> **Invariant B (distance monotonicity of the queue):**
> Vertices are dequeued in non-decreasing order of `dist`.
>
> Equivalently: BFS fully processes all vertices at distance `d` before dequeuing any vertex at distance `d + 1`.

**Why Invariant B holds:**

- The start vertex is enqueued first with distance `0`.
- New vertices are only enqueued as neighbors of a dequeued vertex `v`, and they always receive distance `dist[v] + 1`.
- The queue is FIFO, so all vertices discovered from distance `d` are appended behind already enqueued vertices of distance `d`, and no vertex at distance `d + 2` can be enqueued before some vertex at distance `d + 1`.

---

## Termination

A vertex is enqueued only when its `dist` changes from `-1` to a non-negative value.

Because `dist[v]` is assigned at most once per vertex, each vertex is enqueued/dequeued at most once. Scanning adjacency lists is finite, so the algorithm terminates.

---

## Shortest path correctness

Fix a start vertex `s` and any vertex `x` reachable from `s`.

Let `d = dist[x]` in the final output.

### 1) Existence of a path of length `d`

By Invariant A, whenever we set `parent[to] = v` we also set `dist[to] = dist[v] + 1`.
Following `parent` pointers from `x` back to `s` yields a path. The distances decrease by 1 at each step, so the path length is exactly `d`.

### 2) Minimality (no shorter path exists)

Consider the moment `x` is first discovered. It is discovered from some dequeued vertex `v` (the algorithm only scans edges from dequeued vertices).

By Invariant B, `v` is processed in increasing distance order, so there is no undiscovered vertex with distance smaller than `dist[v]` left to process.

Suppose there were a path from `s` to `x` of length `< dist[v] + 1`. Then the predecessor of `x` on that path would have distance `< dist[v]` and would have been dequeued earlier, and would have discovered `x` earlier — contradiction.

Therefore `dist[x] = dist[v] + 1` equals the true shortest-path distance from `s` to `x`.

---

## Full coverage (forest version)

`bfs_forest` runs the inner BFS from every vertex that is still unreached (`dist == -1`) in the outer loop.

Each inner BFS discovers exactly the vertices reachable from that root, and because roots are picked until no unreached vertices remain, every vertex is discovered exactly once.

---

## Complexity

- Each vertex is enqueued/dequeued at most once: `O(V)` queue operations.
- Each directed edge `v -> to` is inspected at most once while scanning `g[v]`: `O(E)`.

Total time is `O(V + E)`, and additional memory is `O(V)`.

