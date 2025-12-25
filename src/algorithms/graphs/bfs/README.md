# Breadth-First Search (BFS)

This module implements **Breadth-First Search (BFS)** for graphs using a compact **adjacency list** representation.

BFS is the standard traversal for:

- **shortest path distances** in **unweighted** graphs (each edge has cost 1)
- building a **shortest-path tree** (via `parent` pointers)
- checking **reachability / connectivity**
- layer-by-layer processing ("graph levels")

The traversal is **deterministic** given the adjacency list iteration order.

---

## API

Header: `include/algorithms/graphs/bfs/bfs.h`

```cpp
#include <algorithms/graphs/bfs/bfs.h>

using algorithms::graphs::bfs::AdjList;
using algorithms::graphs::bfs::bfs_from;

AdjList g(5);
// 0 -> 1,2 ; 1 -> 3 ; 2 -> 3,4

g[0] = {1, 2};
g[1] = {3};
g[2] = {3, 4};

auto res = bfs_from(g, 0);
// res.order  : discovery order (when a vertex is first discovered)
// res.dist[v]: shortest distance from 0 (or -1 if unreachable)
// res.parent : predecessor on a shortest path (or -1)
```

### Data types

- `AdjList = std::vector<std::vector<int32_t>>`

- `BfsResult` (BFS from a single start):
  - `dist[v]`: shortest distance from `start`, or `-1` if unreachable
  - `parent[v]`: predecessor on a shortest path
    - `-1` for `start`
    - `-1` for unreachable vertices
  - `order`: vertices in the order they are first discovered (enqueued)

- `BfsForest` (BFS over all vertices / disconnected graphs):
  - `dist[v]`: distance from the chosen component root
  - `parent[v]`: parent in the BFS forest (`-1` for component roots)
  - `order`: discovery order over the entire forest

### Functions

- `BfsResult bfs_from(const AdjList& g, int32_t start)`
  - BFS from a single start vertex.
  - If `start` is out of range, returns an empty result.

- `std::vector<int32_t> bfs_order_from(const AdjList& g, int32_t start)`
  - Convenience wrapper returning only `order`.

- `BfsForest bfs_forest(const AdjList& g)`
  - Runs BFS from each still-unreached vertex in increasing id order.
  - Useful for disconnected graphs.

---

## Notes on behavior

- Works for **directed** and **undirected** graphs.
- Edges pointing to invalid vertices (negative / `>= n`) are ignored defensively.
- Shortest-path guarantees apply to **unweighted graphs**.

---

## Complexity

For a graph with `V` vertices and `E` edges:

- Time: `O(V + E)`
- Extra space: `O(V)`

---

## Proof / correctness

See [`proof.md`](./proof.md).

