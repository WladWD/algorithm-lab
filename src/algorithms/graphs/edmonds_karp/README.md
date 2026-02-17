# Edmonds–Karp Algorithm

## Overview

The **Edmonds–Karp algorithm** is an implementation of the Ford–Fulkerson method for computing
the **maximum flow** in a flow network. It uses **Breadth-First Search (BFS)** to find
augmenting paths, which guarantees polynomial time complexity.

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

The Edmonds–Karp algorithm improves the Ford–Fulkerson method by always choosing the
**shortest augmenting path** (in terms of number of edges) using BFS. This ensures:

- The algorithm terminates in O(V · E) iterations
- Each BFS takes O(E) time
- Total time complexity: O(V · E²)

### Steps

1. **Initialize**: Set flow f(u, v) = 0 for all edges
2. **Find augmenting path**: Use BFS to find a path from s to t in the residual graph
   where all edges have positive residual capacity
3. **Augment flow**: Find the bottleneck (minimum residual capacity) along the path
   and push that amount of flow
4. **Update residual graph**: Decrease forward edge capacities and increase reverse
   edge capacities
5. **Repeat**: Until no augmenting path exists
6. **Return**: Sum of all flow augmentations

### Residual Graph

The **residual graph** G_f for flow f contains:

- Original edges (u, v) with residual capacity c_f(u, v) = c(u, v) − f(u, v)
- Reverse edges (v, u) with residual capacity c_f(v, u) = f(u, v)

## When to Use

### ✅ Best suited for

- Networks with moderate size (up to ~10³ vertices, ~10⁴ edges)
- When code simplicity is prioritized over raw performance
- Problems reducible to max-flow (bipartite matching, min-cut, etc.)

### ❌ Consider alternatives when

- Very large networks → Use **Dinic's algorithm** (O(V² · E))
- Dense graphs or specific structures → Use **Push-Relabel**
- Need to find all min-cuts → Additional analysis required

## Complexity

| Metric | Complexity |
|--------|------------|
| **Time** | O(V · E²) |
| **Space** | O(V + E) |
| **BFS iterations** | O(V · E) |

Where:
- V = number of vertices
- E = number of edges

## Interface

```cpp
#include "algorithms/graphs/edmonds_karp/edmonds_karp.h"

using namespace algorithms::graphs::edmonds_karp;

// Method 1: Using FlowNetwork class
FlowNetwork network(n);            // n = number of vertices
network.add_edge(0, 1, 10);        // edge from 0 to 1 with capacity 10
network.add_edge(1, 2, 5);
int64_t max_flow = network.max_flow(source, sink);

// Method 2: Using convenience function
std::vector<std::tuple<int32_t, int32_t, int32_t>> edges = {
    {0, 1, 10},  // from, to, capacity
    {1, 2, 5},
};
int64_t max_flow = compute_max_flow(n, edges, source, sink);
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

| Iteration | Augmenting Path | Bottleneck | Total Flow |
|-----------|-----------------|------------|------------|
| 1 | 0 → 1 → 3 | 10 | 10 |
| 2 | 0 → 2 → 3 | 10 | 20 |

**Maximum Flow = 20**

## Applications

1. **Network routing**: Maximize data throughput
2. **Bipartite matching**: Model as max-flow problem
3. **Project selection**: Find optimal subset with dependencies
4. **Image segmentation**: Min-cut for foreground/background separation
5. **Airline scheduling**: Crew assignment and routing
6. **Baseball elimination**: Determine if a team can still win

## Related Algorithms

| Algorithm | Time Complexity | Notes |
|-----------|-----------------|-------|
| Ford–Fulkerson | O(E · max_flow) | Original, may not terminate for irrational capacities |
| Edmonds–Karp | O(V · E²) | BFS-based, always polynomial |
| Dinic | O(V² · E) | Block flow, faster for unit capacity |
| Push-Relabel | O(V² · E) or O(V³) | Often fastest in practice |
| HLPP | O(V² · √E) | Highest-label push-relabel variant |

## References

1. Edmonds, J.; Karp, R. M. (1972). "Theoretical improvements in algorithmic efficiency
   for network flow problems". *Journal of the ACM*.
2. Cormen, T. H.; Leiserson, C. E.; Rivest, R. L.; Stein, C. — *Introduction to
   Algorithms (CLRS)*, 4th Ed., Chapter 26.
3. Kleinberg, J.; Tardos, É. — *Algorithm Design*, Chapter 7.

## See Also

- [proof.md](./proof.md) — Formal correctness proof and complexity analysis

