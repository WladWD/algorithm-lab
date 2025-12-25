#include <algorithms/graphs/topological_sort/topological_sort.h>

#include <algorithm>
#include <cstddef>
#include <queue>

namespace algorithms::graphs::topological_sort {

TopologicalSortResult topological_sort(const AdjList& g) {
    const int32_t n = static_cast<int32_t>(g.size());

    TopologicalSortResult out;
    out.order.clear();
    out.order.reserve(static_cast<size_t>(n));
    out.has_cycle = false;

    std::vector<int32_t> indeg(static_cast<size_t>(n), 0);

    for (int32_t u = 0; u < n; ++u) {
        const auto su = static_cast<size_t>(u);
        for (int32_t v : g[su]) {
            if (v < 0 || v >= n) {
                continue; // ignore invalid edges defensively
            }
            indeg[static_cast<size_t>(v)]++;
        }
    }

    std::queue<int32_t> q;
    for (int32_t v = 0; v < n; ++v) {
        if (indeg[static_cast<size_t>(v)] == 0) {
            q.push(v);
        }
    }

    while (!q.empty()) {
        const int32_t u = q.front();
        q.pop();
        out.order.push_back(u);

        const auto su = static_cast<size_t>(u);
        for (int32_t v : g[su]) {
            if (v < 0 || v >= n) {
                continue;
            }
            auto& deg = indeg[static_cast<size_t>(v)];
            deg--;
            if (deg == 0) {
                q.push(v);
            }
        }
    }

    if ((int32_t)out.order.size() != n) {
        out.has_cycle = true;
    }
    return out;
}

std::vector<int32_t> topological_order_or_empty(const AdjList& g) {
    auto res = topological_sort(g);
    if (res.has_cycle) {
        return {};
    }
    return std::move(res.order);
}

} // namespace algorithms::graphs::topological_sort

