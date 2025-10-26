#include "algorithms/graphs/dijkstra/dijkstra.h"

#include <queue>
#include <set>

namespace {
constexpr int32_t INF = std::numeric_limits<int32_t>::max();
}

namespace algorithms::graphs::dijkstra {
std::vector<int32_t> dijkstra_queue(const Graph& g, int32_t s) {
    std::vector<int32_t> dist(g.size(), INF);

    using P = std::pair<int32_t, int32_t>;
    std::priority_queue<P, std::vector<P>, std::greater<>> pq;

    dist[s] = 0;
    pq.emplace(0, s);

    while (!pq.empty()) {
        const auto& [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) {
            continue;
        }

        for (const auto& [to, w] : g[u]) {
            if (dist[to] > d + w) {
                dist[to] = d + w;
                pq.emplace(dist[to], to);
            }
        }
    }
    return dist;
}

std::vector<int32_t> dijkstra_set(const Graph& g, int32_t s) {
    std::vector<int32_t> dist(g.size(), INF);

    using P = std::pair<int32_t, int32_t>;
    std::set<P> q;

    dist[s] = 0;
    q.emplace(0, s);

    while (!q.empty()) {
        const auto& [d, u] = *q.begin();
        q.erase(q.begin());

        for (const auto& [to, w] : g[u]) {
            if (dist[to] > d + w) {
                q.erase(std::make_pair(dist[to], to));
                dist[to] = d + w;
                q.emplace(dist[to], to);
            }
        }
    }

    return dist;
}

std::vector<int32_t> dijkstra(const Graph& g, int32_t s) {
    std::vector<int32_t> dist(g.size(), INF);
    std::vector<bool> u(g.size(), false);

    dist[s] = 0;

    for (size_t i = 0; i < g.size(); ++i) {
        int32_t v = -1;
        for (size_t j = 0; j < g.size(); ++j) {
            if (!u[j] && (v == -1 || dist[j] < dist[v])) {
                v = j;
            }
        }

        if (dist[v] == INF) {
            break;
        }

        u[v] = true;
        for (const auto& [to, w] : g[v]) {
            dist[to] = std::min(dist[to], dist[v] + w);
        }
    }

    return dist;
}
} // namespace algorithms::graphs::dijkstra