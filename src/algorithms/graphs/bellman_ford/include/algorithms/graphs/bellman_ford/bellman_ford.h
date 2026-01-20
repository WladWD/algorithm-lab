#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace algorithms::graphs::bellman_ford {

// Edge in a directed weighted graph.
struct Edge {
    static constexpr int32_t Invalid = std::numeric_limits<int32_t>::max();

    int32_t to = Invalid;
    int32_t w = 0;
};

// Adjacency-list representation (directed or undirected).
// For undirected graphs, add edges in both directions.
using Graph = std::vector<std::vector<Edge>>;

struct BellmanFordResult {
    // dist[v] == INF means unreachable from source.
    std::vector<int32_t> dist;

    // parent[v] is a predecessor of v on some shortest path from source.
    // parent[source] == -1
    // parent[v] == -1 for unreachable vertices.
    std::vector<int32_t> parent;

    // True iff there exists a negative cycle reachable from the source.
    bool has_negative_cycle = false;

    static constexpr int32_t INF = std::numeric_limits<int32_t>::max();
};

// Single-source shortest paths for graphs with possible negative weights.
//
// - If a vertex is unreachable, dist[v] == BellmanFordResult::INF and parent[v] == -1.
// - If a negative cycle is reachable from s, has_negative_cycle = true and dist values
//   are not meaningful for vertices that can be improved indefinitely.
//
// Preconditions:
// - 0 <= s < g.size() (otherwise returns an empty result)
//
// Complexity: O(V * E) time, O(V) additional memory.
BellmanFordResult bellman_ford(const Graph& g, int32_t s);

} // namespace algorithms::graphs::bellman_ford

