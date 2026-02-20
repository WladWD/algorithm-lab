#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace algorithms::graphs::dinic {

/**
 * @brief Edge structure for flow networks.
 *
 * Represents a directed edge in a flow network with capacity and flow tracking.
 * Each edge stores a reference to its reverse edge for efficient flow updates.
 */
struct FlowEdge {
    int32_t to;          // Target vertex
    int32_t capacity;    // Maximum capacity of this edge
    int32_t flow;        // Current flow through this edge
    int32_t rev_idx;     // Index of the reverse edge in the adjacency list of 'to'
};

/**
 * @brief Flow network representation using adjacency lists.
 *
 * Contains the graph structure and provides methods for adding edges
 * and computing maximum flow using Dinic's algorithm with level graph
 * construction (BFS) and blocking flow computation (DFS).
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
     * @brief Compute maximum flow from source to sink using Dinic's algorithm.
     * @param source Source vertex.
     * @param sink Sink vertex.
     * @return Maximum flow value.
     *
     * Time complexity: O(V² · E)
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
     * @brief Build the level graph using BFS.
     * @param source Source vertex.
     * @param sink Sink vertex.
     * @return true if the sink is reachable from the source in the residual graph.
     *
     * Populates level_[v] with the BFS distance from source to v.
     * Unreachable vertices get level -1.
     */
    [[nodiscard]] bool bfs(int32_t source, int32_t sink);

    /**
     * @brief Send blocking flow along the level graph using DFS.
     * @param v Current vertex.
     * @param sink Sink vertex.
     * @param pushed Flow pushed so far along the current path.
     * @return Amount of flow successfully pushed to the sink.
     *
     * Uses the iter_ array (current-arc optimization) to avoid
     * revisiting dead-end edges within the same BFS phase.
     */
    [[nodiscard]] int32_t dfs(int32_t v, int32_t sink, int32_t pushed);

    int32_t n_;
    std::vector<std::vector<FlowEdge>> adj_;
    std::vector<int32_t> level_;  // BFS distance from source
    std::vector<int32_t> iter_;   // Current edge pointer per vertex (current-arc optimization)
};

/**
 * @brief Convenience function to compute maximum flow.
 * @param n Number of vertices.
 * @param edges List of edges: {from, to, capacity}.
 * @param source Source vertex.
 * @param sink Sink vertex.
 * @return Maximum flow value.
 */
[[nodiscard]] int64_t compute_max_flow(
    int32_t n,
    const std::vector<std::tuple<int32_t, int32_t, int32_t>>& edges,
    int32_t source,
    int32_t sink);

} // namespace algorithms::graphs::dinic

