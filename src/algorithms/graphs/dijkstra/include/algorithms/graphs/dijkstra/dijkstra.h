#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace algorithms::graphs::dijkstra {
struct Edge {
    static constexpr int32_t Invalid = std::numeric_limits<int32_t>::max();

    int32_t to = Invalid;
    int32_t w = 0;
};

// Adjacency list representation
using Graph = std::vector<std::vector<Edge>>;

std::vector<int32_t> dijkstra_queue(const Graph& g, int32_t s);
std::vector<int32_t> dijkstra_set(const Graph& g, int32_t s);
std::vector<int32_t> dijkstra(const Graph& g, int32_t s);
} // namespace algorithms::graphs::dijkstra