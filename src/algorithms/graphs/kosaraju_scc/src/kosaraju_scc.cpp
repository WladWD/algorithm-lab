#include <algorithm>
#include <algorithms/graphs/kosaraju_scc/kosaraju_scc.h>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace algorithms::graphs::kosaraju_scc {

namespace {

AdjList build_transpose(const AdjList& g) {
    const int32_t n = static_cast<int32_t>(g.size());
    AdjList gt(static_cast<size_t>(n));

    for (int32_t u = 0; u < n; ++u) {
        for (int32_t v : g[static_cast<size_t>(u)]) {
            if (v < 0 || v >= n) {
                continue; // ignore invalid edges defensively
            }
            gt[static_cast<size_t>(v)].push_back(u);
        }
    }

    return gt;
}

// Iterative DFS that appends vertices to `out_order` in postorder.
static void dfs_postorder_iter(const AdjList& g, int32_t start, std::vector<uint8_t>& used,
                               std::vector<int32_t>& out_order) {
    // state: (vertex, next_edge_index)
    struct Frame {
        int32_t v;
        size_t next_i;
    };

    std::vector<Frame> st;
    st.push_back(Frame{start, 0});
    used[static_cast<size_t>(start)] = 1;

    const int32_t n = static_cast<int32_t>(g.size());

    while (!st.empty()) {
        auto& top = st.back();
        const int32_t v = top.v;
        auto& i = top.next_i;

        const auto sv = static_cast<size_t>(v);
        if (i < g[sv].size()) {
            const int32_t to = g[sv][i++];
            if (to < 0 || to >= n) {
                continue;
            }
            const auto sto = static_cast<size_t>(to);
            if (!used[sto]) {
                used[sto] = 1;
                st.push_back(Frame{to, 0});
            }
        } else {
            out_order.push_back(v);
            st.pop_back();
        }
    }
}

// Iterative DFS on transpose that assigns component id.
static void dfs_assign_iter(const AdjList& gt, int32_t start, int32_t comp_id,
                            std::vector<int32_t>& comp_of) {
    std::vector<int32_t> st;
    st.push_back(start);
    comp_of[static_cast<size_t>(start)] = comp_id;

    const int32_t n = static_cast<int32_t>(gt.size());

    while (!st.empty()) {
        const int32_t v = st.back();
        st.pop_back();

        for (int32_t to : gt[static_cast<size_t>(v)]) {
            if (to < 0 || to >= n) {
                continue;
            }
            auto& slot = comp_of[static_cast<size_t>(to)];
            if (slot != -1) {
                continue;
            }
            slot = comp_id;
            st.push_back(to);
        }
    }
}

} // namespace

SccResult kosaraju_scc(const AdjList& g) {
    const int32_t n = static_cast<int32_t>(g.size());

    SccResult res;
    res.component_of.assign(static_cast<size_t>(n), -1);
    res.components_count = 0;

    if (n == 0) {
        return res;
    }

    // Pass 1: compute vertices in decreasing finish time order (postorder).
    std::vector<uint8_t> used(static_cast<size_t>(n), 0);
    std::vector<int32_t> order;
    order.reserve(static_cast<size_t>(n));

    for (int32_t v = 0; v < n; ++v) {
        if (!used[static_cast<size_t>(v)]) {
            dfs_postorder_iter(g, v, used, order);
        }
    }

    // Pass 2: DFS on transpose in reverse finish order, assign components.
    const AdjList gt = build_transpose(g);

    for (int32_t i = n - 1; i >= 0; --i) {
        const int32_t v = order[static_cast<size_t>(i)];
        if (res.component_of[static_cast<size_t>(v)] != -1) {
            continue;
        }
        dfs_assign_iter(gt, v, res.components_count, res.component_of);
        res.components_count++;
    }

    return res;
}

std::vector<std::vector<int32_t>> build_components(const SccResult& scc) {
    const int32_t n = static_cast<int32_t>(scc.component_of.size());
    std::vector<std::vector<int32_t>> comps(static_cast<size_t>(scc.components_count));

    for (int32_t v = 0; v < n; ++v) {
        const int32_t c = scc.component_of[static_cast<size_t>(v)];
        if (c < 0) {
            continue;
        }
        comps[static_cast<size_t>(c)].push_back(v);
    }

    return comps;
}

AdjList build_condensation_graph(const AdjList& g, const SccResult& scc) {
    const int32_t n = static_cast<int32_t>(g.size());
    const int32_t cns = scc.components_count;

    AdjList dag(static_cast<size_t>(cns));

    if (n == 0 || cns == 0) {
        return dag;
    }

    std::vector<std::vector<uint8_t>> seen; // seen[from_comp][to_comp]
    // Avoid O(C^2) memory by using per-component marker array.
    std::vector<int32_t> mark(static_cast<size_t>(cns), -1);

    for (int32_t u = 0; u < n; ++u) {
        const int32_t cu = scc.component_of[static_cast<size_t>(u)];
        if (cu < 0) {
            continue;
        }

        int32_t token = u; // unique per u (sufficient)
        for (int32_t v : g[static_cast<size_t>(u)]) {
            if (v < 0 || v >= n) {
                continue;
            }
            const int32_t cv = scc.component_of[static_cast<size_t>(v)];
            if (cv < 0 || cu == cv) {
                continue;
            }
            if (mark[static_cast<size_t>(cv)] == token) {
                continue;
            }
            mark[static_cast<size_t>(cv)] = token;
            dag[static_cast<size_t>(cu)].push_back(cv);
        }
    }

    for (auto& adj : dag) {
        std::sort(adj.begin(), adj.end());
        adj.erase(std::unique(adj.begin(), adj.end()), adj.end());
    }

    return dag;
}

} // namespace algorithms::graphs::kosaraju_scc
