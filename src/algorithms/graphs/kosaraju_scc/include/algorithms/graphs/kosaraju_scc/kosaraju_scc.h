#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace algorithms::graphs::kosaraju_scc {
// Graph representation: adjacency list of a directed graph.
// Nodes are indexed [0, n-1].
using AdjList = std::vector<std::vector<int32_t>>;

// Result of SCC decomposition.
//
// Invariants (on return):
// - component_of[v] is in [0, components_count-1] for every v.
// - components_count == 0 iff n == 0.
// - Two vertices u and v are in the same SCC iff component_of[u] == component_of[v].
//
// Notes:
// - Intended for directed graphs.
// - Edges pointing to invalid vertices (negative / >= n) are ignored defensively.
// - Component ids are deterministic given vertex ids and adjacency iteration order.
struct SccResult {
    std::vector<int32_t> component_of;
    int32_t components_count = 0;
};

// Computes strongly connected components using Kosaraju's algorithm.
//
// Complexity: O(V + E) time and O(V + E) extra space (transpose + stacks).
SccResult kosaraju_scc(const AdjList& g);

// Convenience helper: builds SCCs as a list of vertex groups (by component id).
//
// The returned list has size == components_count and `components[c]` contains all
// vertices v such that component_of[v] == c.
std::vector<std::vector<int32_t>> build_components(const SccResult& scc);

// Returns a condensation graph of SCCs (a DAG):
// - vertices are SCC ids [0..components_count-1]
// - there is an edge c(u) -> c(v) for each original edge u -> v where c(u) != c(v)
//
// The output has no parallel edges.
AdjList build_condensation_graph(const AdjList& g, const SccResult& scc);

} // namespace algorithms::graphs::kosaraju_scc
