# Floyd–Warshall — Proof of Correctness

## Problem

Given a directed weighted graph on vertices `0..n-1` (possibly with negative edges), compute the shortest path distance `dist[i][j]` for all ordered pairs `(i, j)`.

If the graph contains a **negative cycle** reachable along a path from `i` to `j`, the shortest path distance is not well-defined (it can be decreased indefinitely). Floyd–Warshall detects this by checking whether `dist[v][v] < 0` for some `v`.

---

## DP formulation

Define `D_k[i][j]` as the length of the shortest path from `i` to `j` such that all **intermediate** vertices (if any) are chosen only from the set `{0, 1, ..., k}`.

- Base: `D_{-1}[i][j]` is simply the direct edge weight `w(i, j)` (or `INF` if there is no edge).
- Transition:

For a fixed `k >= 0`, any shortest path from `i` to `j` with intermediates in `{0..k}` either:

1. does **not** use vertex `k` as an intermediate, so its length is `D_{k-1}[i][j]`, or
2. uses `k` at least once; then it can be decomposed into `i -> k` and `k -> j` with intermediates in `{0..k-1}`.

Therefore:

```
D_k[i][j] = min(
  D_{k-1}[i][j],
  D_{k-1}[i][k] + D_{k-1}[k][j]
)
```

The algorithm implements this recurrence in-place by iterating `k` from `0` to `n-1`.

---

## Correctness argument (sketch)

We prove by induction on `k` that after processing `k`, the stored matrix equals `D_k`.

- **Base (`k = -1`)**: before the loops start, `dist[i][j]` is initialized to the input weights, i.e. `D_{-1}[i][j]`.
- **Induction step**: assume the matrix equals `D_{k-1}` when starting iteration `k`.
  For each pair `(i, j)`, we apply exactly the transition above, so the updated value becomes `D_k[i][j]`.

After the final iteration (`k = n-1`), all vertices are allowed as intermediates, so the matrix equals the true all-pairs shortest path distances (when no negative cycles invalidate the notion).

---

## Negative cycle detection

If there exists a negative cycle reachable from some vertex `v`, then there is a walk from `v` back to `v` with negative total weight. Once all vertices are allowed as intermediates, the shortest such walk implies:

- `dist[v][v] < 0` for at least one vertex `v`.

Conversely, if `dist[v][v] < 0`, then there exists a cycle (a closed walk) of negative total weight reachable from `v`, i.e. a negative cycle.

Hence checking the diagonal is a correct detection method.

---

## Path reconstruction

Maintain `next[i][j]` such that it stores the **first hop** on a shortest path from `i` to `j`.

- If there is a direct edge `i -> j`, initialize `next[i][j] = j`.
- When we relax `(i, j)` via `k`, we set:

```
next[i][j] = next[i][k]
```

This preserves the invariant that repeatedly following `next` reconstructs a shortest path.

If a negative cycle exists, shortest paths may not be well-defined; `next` should not be used to produce meaningful shortest routes in that case.

