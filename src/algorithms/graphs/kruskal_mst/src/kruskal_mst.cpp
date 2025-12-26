#include "algorithms/graphs/kruskal_mst/kruskal_mst.h"

#include <algorithm>
#include <numeric>

namespace algorithms::graphs::kruskal_mst {
namespace {

class Dsu {
public:
    explicit Dsu(int32_t n) : parent_(static_cast<size_t>(n)), size_(static_cast<size_t>(n), 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int32_t find(int32_t v) {
        // Iterative path compression.
        int32_t root = v;
        while (parent_[static_cast<size_t>(root)] != root) {
            root = parent_[static_cast<size_t>(root)];
        }
        while (parent_[static_cast<size_t>(v)] != v) {
            int32_t p = parent_[static_cast<size_t>(v)];
            parent_[static_cast<size_t>(v)] = root;
            v = p;
        }
        return root;
    }

    bool unite(int32_t a, int32_t b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return false;
        }
        if (size_[static_cast<size_t>(a)] < size_[static_cast<size_t>(b)]) {
            std::swap(a, b);
        }
        parent_[static_cast<size_t>(b)] = a;
        size_[static_cast<size_t>(a)] += size_[static_cast<size_t>(b)];
        return true;
    }

private:
    std::vector<int32_t> parent_;
    std::vector<int32_t> size_;
};

bool is_valid_vertex(int32_t v, int32_t n) {
    return v >= 0 && v < n;
}

} // namespace

KruskalMstResult kruskal_mst(int32_t n, std::vector<Edge> edges) {
    if (n <= 0) {
        return KruskalMstResult{.vertices = 0, .components = 0, .total_weight = 0, .edges = {}};
    }

    // Filter invalid/self-loop edges early.
    std::vector<Edge> filtered;
    filtered.reserve(edges.size());
    for (const auto& e : edges) {
        if (e.u == e.v) {
            continue;
        }
        if (!is_valid_vertex(e.u, n) || !is_valid_vertex(e.v, n)) {
            continue;
        }
        filtered.push_back(e);
    }

    std::sort(filtered.begin(), filtered.end(), [](const Edge& a, const Edge& b) {
        if (a.w != b.w) {
            return a.w < b.w;
        }
        if (a.u != b.u) {
            return a.u < b.u;
        }
        return a.v < b.v;
    });

    Dsu dsu(n);

    KruskalMstResult res;
    res.vertices = n;
    res.total_weight = 0;
    res.edges.reserve(static_cast<size_t>(n - 1));

    int32_t merges = 0;
    for (const auto& e : filtered) {
        if (dsu.unite(e.u, e.v)) {
            res.edges.push_back(e);
            res.total_weight += e.w;
            ++merges;
            if (merges == n - 1) {
                break; // connected graph MST completed
            }
        }
    }

    // Count components.
    int32_t comps = 0;
    for (int32_t v = 0; v < n; ++v) {
        if (dsu.find(v) == v) {
            ++comps;
        }
    }
    res.components = comps;

    return res;
}

} // namespace algorithms::graphs::kruskal_mst

