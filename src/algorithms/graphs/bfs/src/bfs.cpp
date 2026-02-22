#include <algorithms/graphs/bfs/bfs.h>
#include <cstddef>
#include <queue>

namespace algorithms::graphs::bfs {

BfsResult bfs_from(const AdjList& g, int32_t start) {
    const int32_t n = static_cast<int32_t>(g.size());
    if (start < 0 || start >= n) {
        return {};
    }

    BfsResult out;
    out.parent.assign(static_cast<size_t>(n), -1);
    out.dist.assign(static_cast<size_t>(n), -1);
    out.order.clear();
    out.order.reserve(static_cast<size_t>(n));

    std::queue<int32_t> q;

    out.dist[static_cast<size_t>(start)] = 0;
    q.push(start);
    out.order.push_back(start);

    while (!q.empty()) {
        const int32_t v = q.front();
        q.pop();

        const auto sv = static_cast<size_t>(v);
        for (int32_t to : g[sv]) {
            if (to < 0 || to >= n) {
                continue; // ignore invalid edges defensively
            }
            const auto sto = static_cast<size_t>(to);
            if (out.dist[sto] != -1) {
                continue;
            }

            out.parent[sto] = v;
            out.dist[sto] = out.dist[sv] + 1;
            q.push(to);
            out.order.push_back(to);
        }
    }

    return out;
}

std::vector<int32_t> bfs_order_from(const AdjList& g, int32_t start) {
    return bfs_from(g, start).order;
}

BfsForest bfs_forest(const AdjList& g) {
    const int32_t n = static_cast<int32_t>(g.size());

    BfsForest out;
    out.parent.assign(static_cast<size_t>(n), -1);
    out.dist.assign(static_cast<size_t>(n), -1);
    out.order.clear();
    out.order.reserve(static_cast<size_t>(n));

    std::queue<int32_t> q;

    for (int32_t start = 0; start < n; ++start) {
        const auto sstart = static_cast<size_t>(start);
        if (out.dist[sstart] != -1) {
            continue;
        }

        // new component root
        out.dist[sstart] = 0;
        out.parent[sstart] = -1;
        q.push(start);
        out.order.push_back(start);

        while (!q.empty()) {
            const int32_t v = q.front();
            q.pop();

            const auto sv = static_cast<size_t>(v);
            for (int32_t to : g[sv]) {
                if (to < 0 || to >= n) {
                    continue;
                }
                const auto sto = static_cast<size_t>(to);
                if (out.dist[sto] != -1) {
                    continue;
                }

                out.parent[sto] = v;
                out.dist[sto] = out.dist[sv] + 1;
                q.push(to);
                out.order.push_back(to);
            }
        }
    }

    return out;
}

} // namespace algorithms::graphs::bfs
