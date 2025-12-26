# Kruskals Algorithm  Minimum Spanning Tree / Forest

This module implements **Kruskals algorithm** for computing a **minimum spanning tree (MST)** of a connected, undirected, weighted graph.

If the input graph is disconnected, the implementation returns a **minimum spanning forest (MSF)**.

## API

Header:

- `#include "algorithms/graphs/kruskal_mst/kruskal_mst.h"`

Main entry-point:

- `KruskalMstResult kruskal_mst(int32_t n, std::vector<Edge> edges);`

### Data structures

- `Edge { u, v, w }`: undirected weighted edge.
- `KruskalMstResult`:
  - `vertices`: input vertex count `n`
  - `components`: number of connected components in the resulting forest
  - `total_weight`: sum of chosen edge weights
  - `edges`: the chosen edges (size is `n - components`)

## Behavior notes

- Vertices are indexed `0..n-1`.
- **Self-loops** (`u == v`) are ignored.
- Edges with endpoints outside `0..n-1` are ignored.
- Parallel edges are allowed; the algorithm naturally picks the lighter one when useful.

## Complexity

- Sorting edges: `O(E log E)`
- DSU operations: `O(E α(V))` (inverse Ackermann, effectively constant)
- Extra memory: `O(V)`

## Proof / correctness

See [`proof.md`](proof.md).

