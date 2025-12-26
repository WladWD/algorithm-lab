# Prim’s Algorithm — Minimum Spanning Tree / Forest

This module implements **Prim’s algorithm** (with a binary heap) for computing a **minimum spanning tree (MST)** of a connected, undirected, weighted graph.

If the input graph is disconnected, the implementation returns a **minimum spanning forest (MSF)** by running Prim from every not-yet-visited vertex.

## API

Header:

- `#include "algorithms/graphs/prim_mst/prim_mst.h"`

Main entry-point:

- `PrimMstResult prim_mst(int32_t n, const std::vector<Edge>& edges);`

### Data structures

- `Edge { u, v, w }`: undirected weighted edge.
- `PrimMstResult`:
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

With a binary heap and an adjacency list:

- Building adjacency list: `O(E)`
- Heap operations: `O((V + E) log E)`
- Extra memory: `O(V + E)`

## Proof / correctness

See [`proof.md`](proof.md).

