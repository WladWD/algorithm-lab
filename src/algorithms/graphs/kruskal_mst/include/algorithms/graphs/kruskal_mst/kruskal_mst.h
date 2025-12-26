#pragma once

#include <cstdint>
#include <vector>

namespace algorithms::graphs::kruskal_mst {

// Undirected weighted edge between vertices u and v.
// Vertices are indexed in [0, n-1].
struct Edge {
    int32_t u{};
    int32_t v{};
    int64_t w{};
};

// Kruskal output for a graph with n vertices.
//
// For connected graphs this is a Minimum Spanning Tree (MST).
// For disconnected graphs this is a Minimum Spanning Forest (MSF).
struct KruskalMstResult {
    int32_t vertices = 0;      // input n
    int32_t components = 0;    // number of components in the resulting forest
    int64_t total_weight = 0;  // sum of weights of chosen edges
    std::vector<Edge> edges;   // chosen edges (size == n - components)
};

// Compute the minimum spanning forest using Kruskal's algorithm.
//
// Input:
// - n: number of vertices
// - edges: undirected edges (u, v, w). Parallel edges are allowed.
//
// Behavior / validation:
// - If n <= 0, returns {vertices=0, components=0, total_weight=0, edges={}}.
// - Self-loops (u == v) are ignored.
// - Edges with endpoints out of range are ignored.
//
// Complexity:
// - O(E log E) time for sorting edges
// - O(V) extra space for DSU
KruskalMstResult kruskal_mst(int32_t n, std::vector<Edge> edges);

} // namespace algorithms::graphs::kruskal_mst
