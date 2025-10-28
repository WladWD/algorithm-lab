#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace algorithms::graphs::bipartite_check {
// Graph representation: adjacency list for an undirected graph, nodes are [0, n-1]
using AdjList = std::vector<std::vector<int32_t>>;

// Result type: optional vector of colors (0/1) if bipartite, std::nullopt otherwise
using ColorResult = std::vector<int32_t>;
using OptionalColorResult = std::optional<ColorResult>;

// BFS-based bipartite check. Returns colors vector with values 0 or 1 when
// the graph is bipartite; otherwise returns std::nullopt.
// Complexity: O(V + E) time, O(V) space.
OptionalColorResult bipartite_bfs(const AdjList& g);

// DFS-based bipartite check (iterative). Also O(V + E) time, O(V) space.
OptionalColorResult bipartite_dfs(const AdjList& g);
} // namespace algorithms::graphs::bipartite_check
