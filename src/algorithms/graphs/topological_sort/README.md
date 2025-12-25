# Topological Sort

This module implements **topological sorting** for **directed graphs**.

A **topological order** of a directed graph is an ordering of vertices such that for every edge
`u -> v`, vertex `u` appears **before** `v`.

A topological order exists **iff** the graph is a **DAG** (Directed Acyclic Graph).

The implementation includes two standard variants:

- **Kahn's algorithm** (in-degree + queue) — good for cycle detection and iterative processing
- **DFS postorder** (reverse exit-time order) — classic approach using DFS coloring for cycle detection

---

## API

Header: `include/algorithms/graphs/topological_sort/topological_sort.h`

```cpp
#include <algorithms/graphs/topological_sort/topological_sort.h>

using algorithms::graphs::topological_sort::AdjList;
using algorithms::graphs::topological_sort::topological_sort;
using algorithms::graphs::topological_sort::topological_sort_dfs;

AdjList g(4);
// 0 -> 1,2 ; 1 -> 3 ; 2 -> 3

g[0] = {1, 2};
g[1] = {3};
g[2] = {3};

auto kahn = topological_sort(g);
auto dfs = topological_sort_dfs(g);
```

### Data types

- `AdjList = std::vector<std::vector<int32_t>>`

- `TopologicalSortResult`
  - `order`: produced order
  - `has_cycle`: `true` iff the input contains a directed cycle

### Functions

- `TopologicalSortResult topological_sort(const AdjList& g)`
  - Kahn's algorithm (in-degree + queue).

- `TopologicalSortResult topological_sort_dfs(const AdjList& g)`
  - DFS postorder (reverse exit times) with DFS-color cycle detection.

- `std::vector<int32_t> topological_order_or_empty(const AdjList& g)`
  - Kahn wrapper returning `{}` on cycles.

- `std::vector<int32_t> topological_order_or_empty_dfs(const AdjList& g)`
  - DFS wrapper returning `{}` on cycles.

---

## Notes on behavior

- Intended for **directed graphs**. (For undirected graphs with edges in both directions, cycles are typical.)
- Edges pointing to invalid vertices (negative / `>= n`) are ignored defensively.
- Deterministic given:
  - vertex ids are considered in increasing order for initial zero in-degree enqueue
  - adjacency list iteration order

---

## Complexity

For a graph with `V` vertices and `E` edges:

- Time: `O(V + E)`
- Extra space: `O(V)`

---

## Proof / correctness

See [`proof.md`](./proof.md).
