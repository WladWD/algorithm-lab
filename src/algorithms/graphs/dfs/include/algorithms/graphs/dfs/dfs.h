#pragma once

#include <cstdint>
#include <vector>

namespace algorithms::graphs::dfs {
// Graph representation: adjacency list. Works for directed or undirected graphs.
// Nodes are indexed [0, n-1].
using AdjList = std::vector<std::vector<int32_t>>;

// Result of a full DFS forest traversal.
//
// Invariants:
// - parent[v] is -1 for DFS roots, otherwise parent[v] is the predecessor in the DFS tree.
// - order is the sequence of vertices in their discovery (preorder) time.
// - tin/tout are timestamps for entry/exit in DFS (0..2*n-1). Unvisited vertices (if any)
//   keep tin/tout = -1.
struct DfsForest {
    std::vector<int32_t> parent;
    std::vector<int32_t> order;
    std::vector<int32_t> tin;
    std::vector<int32_t> tout;
};

// Run a DFS over all vertices (a DFS forest).
// The traversal is deterministic given adjacency list iteration order.
//
// Complexity: O(V + E) time, O(V) extra space.
DfsForest dfs_forest(const AdjList& g);

// Run a DFS from a single start vertex.
//
// If start is out of range, returns an empty order.
//
// Complexity: O(V + E) in the worst case (for reachable subgraph), O(V) space.
std::vector<int32_t> dfs_order_from(const AdjList& g, int32_t start);

// Recursive DFS forest traversal.
//
// Prefer the iterative version for very deep graphs to avoid stack overflows.
//
// Complexity: O(V + E) time, O(V) extra space (plus recursion depth up to O(V)).
DfsForest dfs_forest_recursive(const AdjList& g);

// Recursive DFS from a single start vertex.
// Returns discovery order of the reachable subgraph.
// If start is out of range, returns an empty vector.
std::vector<int32_t> dfs_order_from_recursive(const AdjList& g, int32_t start);

} // namespace algorithms::graphs::dfs
