# Bellman–Ford — Correctness Notes

This note matches the implementation in `src/bellman_ford.cpp`.

We are given a weighted directed graph `G = (V, E)` with `V = {0, 1, ..., n-1}` and edge weights `w(u,v)` that may be negative.

For a fixed source `s`, define the shortest path distance:

- If there is no path from `s` to `v`, then `d(s,v) = +∞`.
- If there is a path and **no reachable negative cycle** can further reduce the path weight, then
  `d(s,v)` is the minimum total weight among all paths from `s` to `v`.

---

## Algorithm

Maintain `dist[]`:
- `dist[s] = 0`
- `dist[v] = +∞` for `v != s`

Repeat `V-1` times:
- For every edge `(u -> v)` with weight `w`, relax:
  - if `dist[u] + w < dist[v]`, set `dist[v] = dist[u] + w` and update `parent[v]`.

Finally, do one more full pass over all edges:
- If any relaxation is still possible from a reachable `u` (i.e. `dist[u] != +∞`), report `has_negative_cycle = true`.

---

## Key lemma: k-edge optimality

Let `dist_k[v]` be the value of `dist[v]` after `k` full relaxation rounds (where a round scans all edges).

> **Lemma:** After `k` rounds, `dist_k[v]` equals the minimum weight of any path from `s` to `v` that uses **at most `k` edges**.

**Proof sketch (by induction on k):**

- Base `k = 0`:
  - Only the empty path to `s` exists with weight 0.
  - All other vertices have no 0-edge path, so distance is `+∞`.

- Step `k -> k+1`:
  - Any path to `v` with at most `k+1` edges ends with some edge `(u -> v)`.
  - The prefix to `u` uses at most `k` edges, so by induction its minimal weight is `dist_k[u]`.
  - Relaxation considers `dist_k[u] + w(u,v)` and takes the minimum over all such `u`, producing exactly the best `(k+1)`-edge-bounded path value.

---

## Correctness when no reachable negative cycles

If there is no reachable negative cycle, then some shortest path from `s` to any reachable `v` can be chosen to be **simple**, hence has at most `V-1` edges.

By the lemma, after `V-1` rounds:
- `dist[v]` equals the minimum weight among all paths from `s` to `v` with at most `V-1` edges
- which equals the true shortest path distance `d(s,v)`.

Unreachable vertices remain at `+∞`.

---

## Negative cycle detection

If after `V-1` rounds we can still relax some edge `(u -> v)` from a reachable `u`, then there exists a walk from `s` to `u` and an edge to `v` that improves the path.

That implies there is a path from `s` to `v` with at least `V` edges whose weight is smaller than the best `V-1`-edge-bounded path. Any walk with `V` edges repeats a vertex, so it contains a cycle.

The only way adding a cycle can strictly improve a path is if that cycle has **negative total weight**.

Therefore:
- an improvement in the extra pass implies a **negative cycle reachable from `s`**.

Conversely, if a reachable negative cycle exists, then traversing it more times decreases path weight without bound, so an additional relaxation will always be possible after `V-1` rounds.

---

## Termination and complexity

- Each round scans all adjacency lists, inspecting each directed edge once, so `O(E)` per round.
- There are `V-1` rounds plus one extra scan, so total time is `O(VE)`.
- Extra memory is `O(V)` for `dist` and `parent`.

