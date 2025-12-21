# Depth-First Search (DFS)

This module implements **Depth-First Search (DFS)** for graphs using a compact **adjacency list** representation.

DFS is a fundamental traversal used for:

- reachability / connectivity
- topological sorting (on DAGs)
- cycle detection
- articulation points / bridges (via DFS timestamps)
- strongly connected components (as a building block)

The implementation is **iterative** (no recursion), so it’s safe for large graphs without risking stack overflows.

The module provides **both iterative and recursive** implementations:

- **Iterative** (default): safe for very deep graphs.
- **Recursive**: compact and easy to read, but recursion depth is `O(V)`.

---

## API

Header: `include/algorithms/graphs/dfs/dfs.h`

```cpp
#include <algorithms/graphs/dfs/dfs.h>

using algorithms::graphs::dfs::AdjList;
using algorithms::graphs::dfs::dfs_forest;

AdjList g(4);
g[0] = {1, 2};
g[1] = {2};
g[2] = {0, 3};

auto res = dfs_forest(g);
// res.order: discovery order
// res.parent: DFS tree parent (-1 for roots)
// res.tin / res.tout: entry/exit timestamps
```

### Data types

- `AdjList = std::vector<std::vector<int32_t>>`
- `DfsForest`:
  - `parent[v]`: parent in the DFS forest (`-1` for a root)
  - `order`: vertices in discovery (preorder) order
  - `tin[v]`: discovery time
  - `tout[v]`: finish time

### Functions

- `DfsForest dfs_forest(const AdjList& g)`
  - Iterative DFS over **all vertices**.

- `std::vector<int32_t> dfs_order_from(const AdjList& g, int32_t start)`
  - Iterative DFS from one start vertex.

- `DfsForest dfs_forest_recursive(const AdjList& g)`
  - Recursive DFS over **all vertices**.

- `std::vector<int32_t> dfs_order_from_recursive(const AdjList& g, int32_t start)`
  - Recursive DFS from one start vertex.

---

## Notes on behavior

- Works for **directed** and **undirected** graphs.
- This DFS is **non-recursive** (explicit stack), which is usually preferred in systems code.
- Edges pointing to invalid vertices (negative / `>= n`) are ignored defensively.

---

## Complexity

For a graph with `V` vertices and `E` edges:

- Time: `O(V + E)`
- Extra space: `O(V)`

---

## Proof / correctness

See [`proof.md`](./proof.md).
