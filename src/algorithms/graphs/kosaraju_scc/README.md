# Kosaraju  Strongly Connected Components (SCC)

This module implements **Kosaraju's algorithm** for finding **strongly connected components**
(SCCs) in a **directed graph**.

A **strongly connected component** is a maximal set of vertices such that every vertex is
reachable from every other vertex in the set.

Kosaraju works in two passes of DFS:

1. Run DFS on the original graph and store vertices in **decreasing finish time** order.
2. Run DFS on the **transpose graph** in that order; each DFS tree gives one SCC.

---

## API

Header: `include/algorithms/graphs/kosaraju_scc/kosaraju_scc.h`

```cpp
#include <algorithms/graphs/kosaraju_scc/kosaraju_scc.h>

using algorithms::graphs::kosaraju_scc::AdjList;
using algorithms::graphs::kosaraju_scc::kosaraju_scc;
using algorithms::graphs::kosaraju_scc::build_components;

AdjList g(5);
// 0 -> 1 -> 2 -> 0 is one SCC, and 3 <-> 4 is another

g[0] = {1};
g[1] = {2};
g[2] = {0};
g[3] = {4};
g[4] = {3};

auto scc = kosaraju_scc(g);
auto comps = build_components(scc);
```

### Data types

- `AdjList = std::vector<std::vector<int32_t>>`

- `SccResult`
  - `component_of[v]`: SCC id for vertex `v`
  - `components_count`: number of SCCs

### Functions

- `SccResult kosaraju_scc(const AdjList& g)`

- `std::vector<std::vector<int32_t>> build_components(const SccResult& scc)`
  - Converts `component_of` mapping into an explicit list of components.

- `AdjList build_condensation_graph(const AdjList& g, const SccResult& scc)`
  - Builds the SCC condensation graph (a DAG) without parallel edges.

---

## Notes on behavior

- Intended for **directed graphs**.
- Edges pointing to invalid vertices (negative / `>= n`) are ignored defensively.
- Component ids are deterministic given:
  - vertex iteration in increasing order
  - adjacency list iteration order

---

## Complexity

For a graph with `V` vertices and `E` edges:

- Time: `O(V + E)`
- Extra space: `O(V + E)` (transpose + DFS stacks)

---

## Proof / correctness

See [`proof.md`](./proof.md).

