#include <algorithms/graphs/topological_sort/topological_sort.h>

#include <algorithm>
#include <cstddef>
#include <queue>

namespace algorithms::graphs::topological_sort {

namespace {
enum class Color : uint8_t {
    White = 0,
    Gray = 1,
    Black = 2,
};

static void dfs_visit(const AdjList& g,
                      int32_t v,
                      std::vector<Color>& color,
                      std::vector<int32_t>& out,
                      bool& has_cycle) {
    if (has_cycle) {
        return;
    }

    color[static_cast<size_t>(v)] = Color::Gray;

    const int32_t n = static_cast<int32_t>(g.size());
    for (int32_t to : g[static_cast<size_t>(v)]) {
        if (to < 0 || to >= n) {
            continue; // ignore invalid edges defensively
        }

        const auto sto = static_cast<size_t>(to);
        if (color[sto] == Color::Gray) {
            has_cycle = true; // back-edge
            return;
        }
        if (color[sto] == Color::White) {
            dfs_visit(g, to, color, out, has_cycle);
            if (has_cycle) {
                return;
            }
        }
    }

    color[static_cast<size_t>(v)] = Color::Black;
    out.push_back(v); // postorder
}
} // namespace

TopologicalSortResult topological_sort_dfs(const AdjList& g) {
    const int32_t n = static_cast<int32_t>(g.size());

    TopologicalSortResult res;
    res.order.clear();
    res.order.reserve(static_cast<size_t>(n));
    res.has_cycle = false;

    std::vector<Color> color(static_cast<size_t>(n), Color::White);

    for (int32_t v = 0; v < n; ++v) {
        if (color[static_cast<size_t>(v)] != Color::White) {
            continue;
        }
        dfs_visit(g, v, color, res.order, res.has_cycle);
        if (res.has_cycle) {
            // Keep partial postorder in res.order, consistent with Kahn variant behavior
            // (order.size() < n).
            return res;
        }
    }

    std::reverse(res.order.begin(), res.order.end());
    return res;
}

std::vector<int32_t> topological_order_or_empty_dfs(const AdjList& g) {
    auto res = topological_sort_dfs(g);
    if (res.has_cycle) {
        return {};
    }
    return std::move(res.order);
}

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

