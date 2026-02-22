#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace algorithms::graphs::edmonds_karp {

/**
 * @brief Edge structure for flow networks.
 *
 * Represents a directed edge in a flow network with capacity and flow tracking.
 * Each edge stores a reference to its reverse edge for efficient flow updates.
 */
struct FlowEdge {
    int32_t to;       // Target vertex
    int32_t capacity; // Maximum capacity of this edge
    int32_t flow;     // Current flow through this edge
    int32_t rev_idx;  // Index of the reverse edge in the adjacency list of 'to'
};

/**
 * @brief Flow network representation using adjacency lists.
 *
 * Contains the graph structure and provides methods for adding edges
 * and computing maximum flow.
 */
class FlowNetwork {
  public:
    /**
     * @brief Construct a flow network with n vertices.
     * @param n Number of vertices in the network.
     */
    explicit FlowNetwork(int32_t n);

    /**
     * @brief Add a directed edge with given capacity.
     * @param from Source vertex.
     * @param to Target vertex.
     * @param capacity Edge capacity (must be non-negative).
     *
     * Automatically creates the reverse edge with capacity 0 for residual graph.
     */
    void add_edge(int32_t from, int32_t to, int32_t capacity);

    /**
     * @brief Compute maximum flow from source to sink using Edmonds-Karp algorithm.
     * @param source Source vertex.
     * @param sink Sink vertex.
     * @return Maximum flow value.
     *
     * Time complexity: O(V * E^2)
     * Space complexity: O(V + E)
     */
    [[nodiscard]] int64_t max_flow(int32_t source, int32_t sink);

    /**
     * @brief Get the residual capacity of an edge after max_flow computation.
     * @param from Source vertex.
     * @param to Target vertex.
     * @return Residual capacity, or -1 if edge doesn't exist.
     */
    [[nodiscard]] int32_t get_flow(int32_t from, int32_t to) const;

    /**
     * @brief Get the number of vertices in the network.
     * @return Number of vertices.
     */
    [[nodiscard]] int32_t size() const { return n_; }

    /**
     * @brief Get the adjacency list for a vertex.
     * @param v Vertex index.
     * @return Reference to the adjacency list.
     */
    [[nodiscard]] const std::vector<FlowEdge>& adj(int32_t v) const { return adj_[v]; }

  private:
    /**
     * @brief Find an augmenting path using BFS.
     * @param source Source vertex.
     * @param sink Sink vertex.
     * @param parent Output: parent[v] = {u, edge_index} for path reconstruction.
     * @return true if an augmenting path exists.
     */
    [[nodiscard]] bool bfs(int32_t source, int32_t sink,
                           std::vector<std::pair<int32_t, int32_t>>& parent) const;

    int32_t n_;
    std::vector<std::vector<FlowEdge>> adj_;
};

/**
 * @brief Convenience function to compute maximum flow.
 * @param n Number of vertices.
 * @param edges List of edges: {from, to, capacity}.
 * @param source Source vertex.
 * @param sink Sink vertex.
 * @return Maximum flow value.
 */
[[nodiscard]] int64_t
compute_max_flow(int32_t n, const std::vector<std::tuple<int32_t, int32_t, int32_t>>& edges,
                 int32_t source, int32_t sink);

} // namespace algorithms::graphs::edmonds_karp
