#include "algorithms/graphs/prim_mst/prim_mst.h"

#include <queue>
#include <vector>

namespace algorithms::graphs::prim_mst {
namespace {

bool is_valid_vertex(int32_t v, int32_t n) {
    return v >= 0 && v < n;
}

struct AdjEdge {
    int32_t to{};
    int64_t w{};
};

struct HeapItem {
    int64_t w{};
    int32_t from{};
    int32_t to{};

    friend bool operator>(const HeapItem& a, const HeapItem& b) {
        if (a.w != b.w) {
            return a.w > b.w;
        }
        if (a.from != b.from) {
            return a.from > b.from;
        }
        return a.to > b.to;
    }
};

} // namespace

PrimMstResult prim_mst(int32_t n, const std::vector<Edge>& edges) {
    if (n <= 0) {
        return PrimMstResult{.vertices = 0, .components = 0, .total_weight = 0, .edges = {}};
    }

    // Build adjacency list; ignore self-loops and invalid endpoints (mirrors Kruskal module rules).
    std::vector<std::vector<AdjEdge>> g(static_cast<size_t>(n));
    g.shrink_to_fit();
    
    for (const auto& e : edges) {
        if (e.u == e.v) {
            continue;
        }
        if (!is_valid_vertex(e.u, n) || !is_valid_vertex(e.v, n)) {
            continue;
        }
        g[static_cast<size_t>(e.u)].push_back(AdjEdge{.to = e.v, .w = e.w});
        g[static_cast<size_t>(e.v)].push_back(AdjEdge{.to = e.u, .w = e.w});
    }

    PrimMstResult res;
    res.vertices = n;
    res.total_weight = 0;
    res.edges.reserve(static_cast<size_t>(n - 1));

    std::vector<char> used(static_cast<size_t>(n), 0);

    // We run Prim from every not-yet-visited vertex => MSF.
    for (int32_t start = 0; start < n; ++start) {
        if (used[static_cast<size_t>(start)] != 0) {
            continue;
        }

        ++res.components;
        used[static_cast<size_t>(start)] = 1;

        std::priority_queue<HeapItem, std::vector<HeapItem>, std::greater<>> pq;
        for (const auto& ae : g[static_cast<size_t>(start)]) {
            pq.push(HeapItem{.w = ae.w, .from = start, .to = ae.to});
        }

        while (!pq.empty()) {
            auto cur = pq.top();
            pq.pop();

            if (used[static_cast<size_t>(cur.to)] != 0) {
                continue;
            }

            // Take this edge.
            used[static_cast<size_t>(cur.to)] = 1;
            res.edges.push_back(Edge{.u = cur.from, .v = cur.to, .w = cur.w});
            res.total_weight += cur.w;

            // Add outgoing edges.
            for (const auto& ae : g[static_cast<size_t>(cur.to)]) {
                if (used[static_cast<size_t>(ae.to)] == 0) {
                    pq.push(HeapItem{.w = ae.w, .from = cur.to, .to = ae.to});
                }
            }
        }
    }

    return res;
}

} // namespace algorithms::graphs::prim_mst

