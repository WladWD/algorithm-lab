#include "algorithms/graphs/dinic/dinic.h"

#include <algorithm>
#include <queue>

namespace algorithms::graphs::dinic {

FlowNetwork::FlowNetwork(int32_t n) : n_(n), adj_(n), level_(n), iter_(n) {}

void FlowNetwork::add_edge(int32_t from, int32_t to, int32_t capacity) {
    // Add forward edge with given capacity
    adj_[from].push_back({to, capacity, 0, static_cast<int32_t>(adj_[to].size())});
    // Add reverse edge with 0 capacity (for residual graph)
    adj_[to].push_back({from, 0, 0, static_cast<int32_t>(adj_[from].size()) - 1});
}

bool FlowNetwork::bfs(int32_t source, int32_t sink) {
    // Build level graph: assign BFS distance from source to every reachable vertex
    std::fill(level_.begin(), level_.end(), -1);
    level_[source] = 0;

    std::queue<int32_t> q;
    q.push(source);

    while (!q.empty()) {
        int32_t u = q.front();
        q.pop();

        for (const auto& e : adj_[u]) {
            // Only traverse edges with positive residual capacity
            if (level_[e.to] == -1 && e.capacity > e.flow) {
                level_[e.to] = level_[u] + 1;
                q.push(e.to);
            }
        }
    }

    // Sink is reachable if it was assigned a level
    return level_[sink] != -1;
}

int32_t FlowNetwork::dfs(int32_t v, int32_t sink, int32_t pushed) {
    if (v == sink) {
        return pushed;
    }

    // Current-arc optimization: resume from where we left off
    for (int32_t& i = iter_[v]; i < static_cast<int32_t>(adj_[v].size()); ++i) {
        auto& e = adj_[v][i];

        // Only follow edges that go to the next level and have residual capacity
        if (level_[e.to] != level_[v] + 1 || e.capacity <= e.flow) {
            continue;
        }

        int32_t d = dfs(e.to, sink, std::min(pushed, e.capacity - e.flow));
        if (d > 0) {
            // Augment flow along this edge and its reverse
            e.flow += d;
            adj_[e.to][e.rev_idx].flow -= d;
            return d;
        }
    }

    return 0;
}

int64_t FlowNetwork::max_flow(int32_t source, int32_t sink) {
    if (source == sink) {
        return 0;
    }

    int64_t total_flow = 0;

    // Phase loop: build level graph, then find blocking flows
    while (bfs(source, sink)) {
        // Reset current-arc pointers for each BFS phase
        std::fill(iter_.begin(), iter_.end(), 0);

        // Send blocking flow: repeatedly DFS until no more augmenting paths
        while (true) {
            int32_t pushed = dfs(source, sink, std::numeric_limits<int32_t>::max());
            if (pushed == 0) {
                break;
            }
            total_flow += pushed;
        }
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

int64_t compute_max_flow(int32_t n, const std::vector<std::tuple<int32_t, int32_t, int32_t>>& edges,
                         int32_t source, int32_t sink) {

    FlowNetwork network(n);
    for (const auto& [from, to, capacity] : edges) {
        network.add_edge(from, to, capacity);
    }
    return network.max_flow(source, sink);
}

} // namespace algorithms::graphs::dinic
