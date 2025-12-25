#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace algorithms::graphs::topological_sort {
// Graph representation: adjacency list of a directed graph.
// Nodes are indexed [0, n-1].
using AdjList = std::vector<std::vector<int32_t>>;

// Result of a topological sort.
//
// Invariants (on return):
// - If has_cycle == false then order is a permutation of [0..n-1] and is a valid
//   topological ordering: for every edge u -> v (with valid endpoints), u appears
//   before v in order.
// - If has_cycle == true then the graph is not a DAG, and order contains a
//   topological ordering of the acyclic part (i.e., the vertices removed by Kahn's
//   algorithm). In particular, order.size() < n.
//
// Notes:
// - Edges pointing to invalid vertices (negative / >= n) are ignored defensively.
// - The algorithm is deterministic given vertex ids and adjacency iteration order.
struct TopologicalSortResult {
    std::vector<int32_t> order;
    bool has_cycle = false;
};

// Computes a topological ordering of a directed graph using Kahn's algorithm
// (in-degree + queue).
//
// If the graph contains a directed cycle, returns has_cycle=true.
//
// Complexity: O(V + E) time, O(V) memory.
TopologicalSortResult topological_sort(const AdjList& g);

// Convenience wrapper: returns only the ordering.
//
// If the graph has a cycle, returns an empty vector.
std::vector<int32_t> topological_order_or_empty(const AdjList& g);

} // namespace algorithms::graphs::topological_sort

