# Dinic's Algorithm

## Overview

**Dinic's algorithm** (also known as Dinitz's algorithm) computes the **maximum flow**
in a flow network. It improves on Edmonds–Karp by constructing a **level graph** via BFS
and then finding **blocking flows** via DFS with the **current-arc optimization**, yielding
a worst-case time complexity of **O(V² · E)**.

## Problem Definition

Given a **directed graph** G = (V, E) representing a flow network with:

- A **source** vertex s ∈ V
- A **sink** vertex t ∈ V
- A **capacity function** c : E → ℤ⁺

Find the maximum amount of flow that can be sent from s to t such that:

1. **Capacity constraint**: For each edge (u, v), flow f(u, v) ≤ c(u, v)
2. **Flow conservation**: For each vertex v ∉ {s, t}, incoming flow = outgoing flow
3. **Skew symmetry**: f(u, v) = −f(v, u)

## Algorithm Description

### Key Insight

Instead of finding one augmenting path per iteration (like Edmonds–Karp), Dinic's algorithm
finds a **blocking flow** — the maximum set of augmenting paths that can be pushed through
the **level graph** — in each phase. The level graph is a DAG built by BFS that only retains
edges going from one BFS level to the next. This structure guarantees that:

- The shortest-path distance from s to t **strictly increases** after each phase
- The algorithm terminates in at most **O(V)** phases
- Each phase takes **O(V · E)** time with the current-arc optimization
- Total time complexity: **O(V² · E)**

### Steps

1. **Build level graph (BFS)**: Run BFS from s. Assign each reachable vertex a
   *level* (BFS distance from s). Discard edges that do not go from level ℓ to level ℓ+1.
2. **Find blocking flow (DFS)**: Use DFS with the *current-arc optimization* to push
   flow along the level graph until no more augmenting paths remain.
   - The current-arc optimization keeps a pointer `iter[v]` for each vertex, so once
     an edge is determined to be a dead end, it is never revisited in the same phase.
3. **Repeat**: Go back to step 1. If BFS cannot reach t, terminate.
4. **Return**: Sum of all flow pushed across all phases.

### Residual Graph

The **residual graph** G_f for flow f contains:

- Original edges (u, v) with residual capacity c_f(u, v) = c(u, v) − f(u, v)
- Reverse edges (v, u) with residual capacity c_f(v, u) = f(u, v)

### Level Graph

The **level graph** L_f is the subgraph of G_f containing only edges (u, v) where:

level(v) = level(u) + 1

This ensures all paths in L_f are shortest paths from s to t.

### Blocking Flow

A **blocking flow** in L_f is a flow such that every path from s to t in L_f contains
at least one saturated edge (residual capacity = 0).

## When to Use

### ✅ Best suited for

- Networks of any size — faster than Edmonds–Karp for large or dense graphs
- **Unit-capacity networks** — runs in O(E · √V), optimal for bipartite matching
- Problems reducible to max-flow (bipartite matching, min-cut, etc.)
- When a clean, efficient implementation is needed

### ❌ Consider alternatives when

- Very dense graphs with specific structure → Use **Push-Relabel** (O(V³) or O(V²√E))
- Only a simple/small network is needed → Edmonds–Karp may be simpler to implement
- Need to enumerate all min-cuts → Additional analysis required

## Complexity

| Metric | Complexity |
|--------|------------|
| **Time (general)** | O(V² · E) |
| **Time (unit-capacity)** | O(E · √V) |
| **Space** | O(V + E) |
| **BFS phases** | O(V) |

Where:
- V = number of vertices
- E = number of edges

## Interface

```cpp
#include "algorithms/graphs/dinic/dinic.h"

using namespace algorithms::graphs::dinic;

// Method 1: Using FlowNetwork class
FlowNetwork network(n);            // n = number of vertices
network.add_edge(0, 1, 10);        // edge from 0 to 1 with capacity 10
network.add_edge(1, 2, 5);
int64_t flow = network.max_flow(source, sink);

// Method 2: Using convenience function
std::vector<std::tuple<int32_t, int32_t, int32_t>> edges = {
    {0, 1, 10},  // from, to, capacity
    {1, 2, 5},
};
int64_t flow = compute_max_flow(n, edges, source, sink);
```

## Example

### Network

```
        10
    0 -----> 1
    |        |
  10|        |10
    v   10   v
    2 -----> 3
```

All edges have capacity 10.

### Execution

| Phase | BFS Levels | Blocking Flow Paths | Flow Pushed | Total Flow |
|-------|------------|---------------------|-------------|------------|
| 1 | {0:0, 1:1, 2:1, 3:2} | 0→1→3 (10), 0→2→3 (10) | 20 | 20 |
| 2 | BFS cannot reach sink | — | 0 | 20 |

**Maximum Flow = 20**

> In phase 1, the level graph contains edges 0→1, 0→2, 1→3, 2→3 (all going from
> one level to the next). The DFS finds two augmenting paths and pushes a total
> blocking flow of 20. In the next BFS, sink 3 is unreachable—algorithm terminates.

## Applications

1. **Network routing**: Maximize data throughput
2. **Bipartite matching**: Model as max-flow (O(E · √V) with unit capacities)
3. **Project selection**: Find optimal subset with dependencies
4. **Image segmentation**: Min-cut for foreground/background separation
5. **Airline scheduling**: Crew assignment and routing
6. **Baseball elimination**: Determine if a team can still win

## Related Algorithms

| Algorithm | Time Complexity | Notes |
|-----------|-----------------|-------|
| Ford–Fulkerson | O(E · max_flow) | Original, may not terminate for irrational capacities |
| Edmonds–Karp | O(V · E²) | BFS-based, always polynomial |
| **Dinic** | **O(V² · E)** | **Level graph + blocking flow, O(E√V) for unit capacity** |
| Push-Relabel | O(V² · E) or O(V³) | Often fastest in practice |
| HLPP | O(V² · √E) | Highest-label push-relabel variant |

## References

1. Dinic, E. A. (1970). "Algorithm for solution of a problem of maximum flow in
   a network with power estimation". *Soviet Mathematics Doklady*, 11, 1277–1280.
2. Even, S.; Tarjan, R. E. (1975). "Network flow and testing graph connectivity".
   *SIAM Journal on Computing*, 4(4), 507–518.
3. Cormen, T. H.; Leiserson, C. E.; Rivest, R. L.; Stein, C. — *Introduction to
   Algorithms (CLRS)*, 4th Ed., Chapter 26.
4. Kleinberg, J.; Tardos, É. — *Algorithm Design*, Chapter 7.

## See Also

- [proof.md](./proof.md) — Formal correctness proof and complexity analysis

