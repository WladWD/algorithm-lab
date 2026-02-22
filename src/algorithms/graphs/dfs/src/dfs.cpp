#include <algorithm>
#include <algorithms/graphs/dfs/dfs.h>
#include <cstddef>
#include <stack>
#include <utility>

namespace algorithms::graphs::dfs {
namespace {
struct Frame {
    int32_t v;
    int32_t parent;
    size_t next_idx;
};
} // namespace

DfsForest dfs_forest(const AdjList& g) {
    const int32_t n = static_cast<int32_t>(g.size());

    DfsForest out;
    out.parent.assign(static_cast<size_t>(n), -1);
    out.tin.assign(static_cast<size_t>(n), -1);
    out.tout.assign(static_cast<size_t>(n), -1);
    out.order.clear();
    out.order.reserve(static_cast<size_t>(n));

    int32_t timer = 0;

    std::vector<uint8_t> color(static_cast<size_t>(n), 0); // 0=white, 1=gray, 2=black
    std::stack<Frame> st;

    for (int32_t start = 0; start < n; ++start) {
        if (color[static_cast<size_t>(start)] != 0) {
            continue;
        }

        st.push(Frame{start, -1, 0});
        while (!st.empty()) {
            auto& f = st.top();
            const auto fv = static_cast<size_t>(f.v);

            if (color[fv] == 0) {
                color[fv] = 1;
                out.parent[fv] = f.parent;
                out.tin[fv] = timer++;
                out.order.push_back(f.v);
            }

            if (f.next_idx >= g[fv].size()) {
                color[fv] = 2;
                out.tout[fv] = timer++;
                st.pop();
                continue;
            }

            const int32_t to = g[fv][f.next_idx++];
            if (to < 0 || to >= n) {
                continue; // ignore invalid edges defensively
            }
            const auto fto = static_cast<size_t>(to);
            if (color[fto] == 0) {
                st.push(Frame{to, f.v, 0});
            }
        }
    }

    return out;
}

std::vector<int32_t> dfs_order_from(const AdjList& g, int32_t start) {
    const int32_t n = static_cast<int32_t>(g.size());
    if (start < 0 || start >= n) {
        return {};
    }

    std::vector<int32_t> order;
    order.reserve(static_cast<size_t>(n));

    std::vector<uint8_t> seen(static_cast<size_t>(n), 0);
    std::stack<int32_t> st;

    st.push(start);
    while (!st.empty()) {
        const int32_t v = st.top();
        st.pop();

        const auto sv = static_cast<size_t>(v);
        if (seen[sv]) {
            continue;
        }
        seen[sv] = 1;
        order.push_back(v);

        // Push neighbors in reverse so we visit them in the original adjacency order.
        const auto& adj = g[sv];
        for (auto it = adj.rbegin(); it != adj.rend(); ++it) {
            const int32_t to = *it;
            if (0 <= to && to < n && !seen[static_cast<size_t>(to)]) {
                st.push(to);
            }
        }
    }

    return order;
}

namespace {
void dfs_rec_visit(const AdjList& g, int32_t v, int32_t parent, std::vector<uint8_t>& color,
                   int32_t& timer, DfsForest& out) {
    const int32_t n = static_cast<int32_t>(g.size());
    const auto sv = static_cast<size_t>(v);

    color[sv] = 1;
    out.parent[sv] = parent;
    out.tin[sv] = timer++;
    out.order.push_back(v);

    for (int32_t to : g[sv]) {
        if (to < 0 || to >= n) {
            continue;
        }
        const auto sto = static_cast<size_t>(to);
        if (color[sto] == 0) {
            dfs_rec_visit(g, to, v, color, timer, out);
        }
    }

    color[sv] = 2;
    out.tout[sv] = timer++;
}
} // namespace

DfsForest dfs_forest_recursive(const AdjList& g) {
    const int32_t n = static_cast<int32_t>(g.size());

    DfsForest out;
    out.parent.assign(static_cast<size_t>(n), -1);
    out.tin.assign(static_cast<size_t>(n), -1);
    out.tout.assign(static_cast<size_t>(n), -1);
    out.order.clear();
    out.order.reserve(static_cast<size_t>(n));

    int32_t timer = 0;
    std::vector<uint8_t> color(static_cast<size_t>(n), 0);

    for (int32_t v = 0; v < n; ++v) {
        if (color[static_cast<size_t>(v)] != 0) {
            continue;
        }
        dfs_rec_visit(g, v, -1, color, timer, out);
    }

    return out;
}

namespace {
void dfs_rec_order(const AdjList& g, int32_t v, std::vector<uint8_t>& seen,
                   std::vector<int32_t>& order) {
    const int32_t n = static_cast<int32_t>(g.size());
    const auto sv = static_cast<size_t>(v);

    seen[sv] = 1;
    order.push_back(v);

    for (int32_t to : g[sv]) {
        if (to < 0 || to >= n) {
            continue;
        }
        const auto sto = static_cast<size_t>(to);
        if (!seen[sto]) {
            dfs_rec_order(g, to, seen, order);
        }
    }
}
} // namespace

std::vector<int32_t> dfs_order_from_recursive(const AdjList& g, int32_t start) {
    const int32_t n = static_cast<int32_t>(g.size());
    if (start < 0 || start >= n) {
        return {};
    }

    std::vector<int32_t> order;
    order.reserve(static_cast<size_t>(n));

    std::vector<uint8_t> seen(static_cast<size_t>(n), 0);
    dfs_rec_order(g, start, seen, order);

    return order;
}

} // namespace algorithms::graphs::dfs
