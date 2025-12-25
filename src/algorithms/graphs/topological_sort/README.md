# Topological Sort

This module implements **topological sorting** for **directed graphs**.

A **topological order** of a directed graph is an ordering of vertices such that for every edge
`u -> v`, vertex `u` appears **before** `v`.

A topological order exists **iff** the graph is a **DAG** (Directed Acyclic Graph).

The implementation uses **Kahn's algorithm** (in-degree + queue).

---

## API

Header: `include/algorithms/graphs/topological_sort/topological_sort.h`

```cpp
#include <algorithms/graphs/topological_sort/topological_sort.h>

using algorithms::graphs::topological_sort::AdjList;
using algorithms::graphs::topological_sort::topological_sort;

AdjList g(4);
// 0 -> 1,2 ; 1 -> 3 ; 2 -> 3

g[0] = {1, 2};
g[1] = {3};
g[2] = {3};

auto res = topological_sort(g);
if (!res.has_cycle) {
    // res.order is a topological ordering of all vertices.
}
```

### Data types

- `AdjList = std::vector<std::vector<int32_t>>`

- `TopologicalSortResult`
  - `order`: produced order
  - `has_cycle`: `true` iff the input contains a directed cycle

### Functions

- `TopologicalSortResult topological_sort(const AdjList& g)`
  - Returns `has_cycle=false` and `order.size()==n` for DAGs.
  - Returns `has_cycle=true` and a partial order for cyclic graphs.

- `std::vector<int32_t> topological_order_or_empty(const AdjList& g)`
  - Returns the full order for DAGs.
  - Returns `{}` if the graph has a cycle.

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

