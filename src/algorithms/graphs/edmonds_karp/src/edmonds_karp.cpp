#include "algorithms/graphs/edmonds_karp/edmonds_karp.h"

#include <algorithm>
#include <queue>

namespace algorithms::graphs::edmonds_karp {

FlowNetwork::FlowNetwork(int32_t n) : n_(n), adj_(n) {}

void FlowNetwork::add_edge(int32_t from, int32_t to, int32_t capacity) {
    // Add forward edge with given capacity
    adj_[from].push_back({to, capacity, 0, static_cast<int32_t>(adj_[to].size())});
    // Add reverse edge with 0 capacity (for residual graph)
    adj_[to].push_back({from, 0, 0, static_cast<int32_t>(adj_[from].size()) - 1});
}

bool FlowNetwork::bfs(int32_t source, int32_t sink,
                      std::vector<std::pair<int32_t, int32_t>>& parent) const {
    // parent[v] = {u, edge_index} means we reached v from u via edge adj_[u][edge_index]
    std::fill(parent.begin(), parent.end(), std::make_pair(-1, -1));

    std::queue<int32_t> q;
    q.push(source);
    parent[source] = {source, -1}; // Mark source as visited

    while (!q.empty()) {
        int32_t u = q.front();
        q.pop();

        for (int32_t i = 0; i < static_cast<int32_t>(adj_[u].size()); ++i) {
            const auto& e = adj_[u][i];
            // Check if vertex is unvisited and has residual capacity
            if (parent[e.to].first == -1 && e.capacity > e.flow) {
                parent[e.to] = {u, i};
                if (e.to == sink) {
                    return true;
                }
                q.push(e.to);
            }
        }
    }

    return false;
}

int64_t FlowNetwork::max_flow(int32_t source, int32_t sink) {
    int64_t total_flow = 0;
    std::vector<std::pair<int32_t, int32_t>> parent(n_);

    // While there exists an augmenting path from source to sink
    while (bfs(source, sink, parent)) {
        // Find minimum residual capacity along the path (bottleneck)
        int32_t path_flow = std::numeric_limits<int32_t>::max();

        for (int32_t v = sink; v != source; ) {
            auto [u, edge_idx] = parent[v];
            path_flow = std::min(path_flow, adj_[u][edge_idx].capacity - adj_[u][edge_idx].flow);
            v = u;
        }

        // Update flow along the path (and reverse edges)
        for (int32_t v = sink; v != source; ) {
            auto [u, edge_idx] = parent[v];
            adj_[u][edge_idx].flow += path_flow;
            adj_[v][adj_[u][edge_idx].rev_idx].flow -= path_flow;
            v = u;
        }

        total_flow += path_flow;
    }

    return total_flow;
}

int32_t FlowNetwork::get_flow(int32_t from, int32_t to) const {
    for (const auto& e : adj_[from]) {
        if (e.to == to) {
            return e.flow;
        }
    }
    return -1; // Edge not found
}

int64_t compute_max_flow(
    int32_t n,
    const std::vector<std::tuple<int32_t, int32_t, int32_t>>& edges,
    int32_t source,
    int32_t sink) {

    FlowNetwork network(n);
    for (const auto& [from, to, capacity] : edges) {
        network.add_edge(from, to, capacity);
    }
    return network.max_flow(source, sink);
}

} // namespace algorithms::graphs::edmonds_karp

