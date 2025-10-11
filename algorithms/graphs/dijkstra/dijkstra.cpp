#include "algorithms/graphs/dijkstra/dijkstra.h"

namespace algorithms::graphs::dijkstra {
std::vector<int> dijkstra(const Graph& g, int s) {
    //todo: reimpl
    const int INF = std::numeric_limits<int>::max()/4;
    std::vector<int> dist(g.size(), INF);
    using P = std::pair<int,int>;
    std::priority_queue<P, std::vector<P>, std::greater<>> pq;
    dist[s] = 0; pq.emplace(0, s);
    while (!pq.empty()) {
        auto [d,u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto e: g[u]) {
            if (dist[e.to] > d + e.w) {
                dist[e.to] = d + e.w;
                pq.emplace(dist[e.to], e.to);
            }
        }
    }
    return dist;
}
} // namespace algorithms::graphs::dijkstra