#pragma once

#include <cstdint>
#include <vector>

namespace algorithms::graphs::bfs {
// Graph representation: adjacency list. Works for directed or undirected graphs.
// Nodes are indexed [0, n-1].
using AdjList = std::vector<std::vector<int32_t>>;

// Result of a BFS traversal from a single start vertex.
//
// Invariants (on return):
// - dist[v] == -1 iff v is unreachable from start.
// - If dist[v] != -1 and v != start then parent[v] is the predecessor on a shortest path.
// - dist[start] == 0 and parent[start] == -1.
// - order is vertices in the order they are first discovered (enqueued).
struct BfsResult {
    std::vector<int32_t> parent;
    std::vector<int32_t> dist;
    std::vector<int32_t> order;
};

// Run BFS from a single start vertex.
//
// If start is out of range, returns an empty order and empty parent/dist.
//
// Complexity: O(V + E) time over the reachable subgraph; O(V) extra space.
BfsResult bfs_from(const AdjList& g, int32_t start);

// Return only the discovery order of BFS from start.
// If start is out of range, returns an empty vector.
std::vector<int32_t> bfs_order_from(const AdjList& g, int32_t start);

// Full BFS forest over all vertices.
//
// This runs BFS starting from each still-unseen vertex in increasing vertex id order.
// Useful to cover a disconnected graph.
//
// Invariants:
// - parent[v] == -1 for a BFS root of its component.
// - dist[v] is distance from its component root.
// - order is discovery order over the entire forest.
struct BfsForest {
    std::vector<int32_t> parent;
    std::vector<int32_t> dist;
    std::vector<int32_t> order;
};

// Complexity: O(V + E) time, O(V) extra space.
BfsForest bfs_forest(const AdjList& g);

} // namespace algorithms::graphs::bfs
