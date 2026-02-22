#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace algorithms::graphs::floyd_warshall {

struct FloydWarshallResult {
    // dist[i][j] is the shortest path distance from i to j, or INF if unreachable.
    std::vector<std::vector<int32_t>> dist;

    // next[i][j] is the next vertex after i on a shortest path to j.
    // If next[i][j] == -1, j is unreachable from i.
    std::vector<std::vector<int32_t>> next;

    // True iff the graph contains a negative cycle.
    // Practically, this means some dist[v][v] < 0 after running the algorithm.
    bool has_negative_cycle = false;

    static constexpr int32_t INF = std::numeric_limits<int32_t>::max();
};

// Floyd–Warshall all-pairs shortest paths.
//
// Input:
// - weights: adjacency matrix where weights[i][j] is the edge weight i -> j.
//   Use INF (FloydWarshallResult::INF) to represent "no edge".
//   Typically, weights[i][i] should be 0.
//
// Output:
// - dist: all-pairs shortest path distances (INF if unreachable)
// - next: path reconstruction table (first hop)
// - has_negative_cycle: true iff there exists any negative cycle
//
// Preconditions:
// - weights is a square matrix (otherwise returns an empty result)
//
// Complexity: O(n^3) time, O(n^2) memory.
FloydWarshallResult floyd_warshall(const std::vector<std::vector<int32_t>>& weights);

} // namespace algorithms::graphs::floyd_warshall
