# Bellman–Ford (Single-Source Shortest Paths)

This module implements **Bellman–Ford** for **directed weighted graphs**, supporting **negative edge weights** and detecting **reachable negative cycles**.

Use this when:
- You have negative weights (Dijkstra is not valid).
- You want to detect a negative cycle reachable from the source.

If all weights are non-negative, prefer **Dijkstra** (`O((V+E) log V)` vs `O(VE)`).

---

## API

Header: `include/algorithms/graphs/bellman_ford/bellman_ford.h`

```cpp
#include <algorithms/graphs/bellman_ford/bellman_ford.h>

using algorithms::graphs::bellman_ford::Graph;
using algorithms::graphs::bellman_ford::Edge;
using algorithms::graphs::bellman_ford::bellman_ford;

Graph g(5);
// directed edges: u -> v (w)
g[0] = {{1, 5}, {2, 2}};
g[2] = {{1, -3}};

auto res = bellman_ford(g, 0);
if (!res.has_negative_cycle) {
    // res.dist[v] is the shortest path distance from 0 to v, or INF if unreachable
}
```

### Data types

- `Edge { to, w }`
- `Graph = std::vector<std::vector<Edge>>` (adjacency list)
- `BellmanFordResult`:
  - `dist[v]`: shortest distance from `s` (or `INF` if unreachable)
  - `parent[v]`: predecessor on a shortest path (`-1` for `s` and unreachable vertices)
  - `has_negative_cycle`: true iff there exists a **negative cycle reachable from `s`**

---

## Behavior notes

- Works for directed and undirected graphs.
  - For an undirected edge `(u, v, w)`, add both `u -> v` and `v -> u`.
- Edges to invalid vertices are ignored defensively.
- If `has_negative_cycle == true`, distances for vertices influenced by that cycle are not well-defined.

---

## Complexity

Let `V = |V|`, `E = |E|`.

- Time: `O(V * E)`
- Extra memory: `O(V)`

---

## Proof / correctness

See [`proof.md`](./proof.md).

