#include "algorithms/graphs/prim_mst/prim_mst.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <numeric>
#include <tuple>
#include <vector>

using namespace algorithms::graphs::prim_mst;

namespace {

struct TestDsu {
    explicit TestDsu(int n) : p(static_cast<size_t>(n)), sz(static_cast<size_t>(n), 1) {
        std::iota(p.begin(), p.end(), 0);
    }

    int find(int v) {
        if (p[static_cast<size_t>(v)] == v) {
            return v;
        }
        p[static_cast<size_t>(v)] = find(p[static_cast<size_t>(v)]);
        return p[static_cast<size_t>(v)];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return false;
        }
        if (sz[static_cast<size_t>(a)] < sz[static_cast<size_t>(b)]) {
            std::swap(a, b);
        }
        p[static_cast<size_t>(b)] = a;
        sz[static_cast<size_t>(a)] += sz[static_cast<size_t>(b)];
        return true;
    }

    std::vector<int> p;
    std::vector<int> sz;
};

bool is_forest(int n, const std::vector<Edge>& edges) {
    TestDsu dsu(n);
    for (const auto& e : edges) {
        if (!dsu.unite(e.u, e.v)) {
            return false; // cycle
        }
    }
    return true;
}

int components_after_edges(int n, const std::vector<Edge>& edges) {
    TestDsu dsu(n);
    for (const auto& e : edges) {
        dsu.unite(e.u, e.v);
    }
    int comps = 0;
    for (int v = 0; v < n; ++v) {
        if (dsu.find(v) == v) {
            ++comps;
        }
    }
    return comps;
}

} // namespace

TEST(PrimMst, EmptyGraphN0) {
    auto res = prim_mst(0, {});
    EXPECT_EQ(res.vertices, 0);
    EXPECT_EQ(res.components, 0);
    EXPECT_EQ(res.total_weight, 0);
    EXPECT_TRUE(res.edges.empty());
}

TEST(PrimMst, SingleVertex) {
    auto res = prim_mst(1, {});
    EXPECT_EQ(res.vertices, 1);
    EXPECT_EQ(res.components, 1);
    EXPECT_EQ(res.total_weight, 0);
    EXPECT_TRUE(res.edges.empty());
}

TEST(PrimMst, ConnectedDemoGraph) {
    // 0--1 (1)
    // 1--2 (2)
    // 0--2 (10)
    // 2--3 (3)
    // 1--3 (100)
    std::vector<Edge> edges = {
        {0, 1, 1}, {1, 2, 2}, {0, 2, 10}, {2, 3, 3}, {1, 3, 100},
    };

    auto res = prim_mst(4, edges);
    EXPECT_EQ(res.vertices, 4);
    EXPECT_EQ(res.components, 1);
    EXPECT_EQ(static_cast<int>(res.edges.size()), 3);
    EXPECT_EQ(res.total_weight, 1 + 2 + 3);
    EXPECT_TRUE(is_forest(4, res.edges));
}

TEST(PrimMst, DisconnectedGraphReturnsForest) {
    // Component A: 0-1-2
    // Component B: 3-4
    std::vector<Edge> edges = {
        {0, 1, 5},
        {1, 2, 1},
        {0, 2, 10},
        {3, 4, 7},
    };

    auto res = prim_mst(5, edges);
    EXPECT_EQ(res.vertices, 5);
    EXPECT_EQ(res.components, 2);
    EXPECT_EQ(static_cast<int>(res.edges.size()), 3); // n - components
    EXPECT_EQ(res.total_weight, 5 + 1 + 7);
    EXPECT_TRUE(is_forest(5, res.edges));
    EXPECT_EQ(components_after_edges(5, res.edges), 2);
}

TEST(PrimMst, ParallelEdgesPickLighter) {
    std::vector<Edge> edges = {
        {0, 1, 10},
        {0, 1, 2},
        {1, 2, 3},
        {0, 2, 100},
    };

    auto res = prim_mst(3, edges);
    EXPECT_EQ(res.components, 1);
    EXPECT_EQ(static_cast<int>(res.edges.size()), 2);
    EXPECT_EQ(res.total_weight, 2 + 3);
}

TEST(PrimMst, SelfLoopsAndInvalidEndpointsIgnored) {
    std::vector<Edge> edges = {
        {0, 0, -100},             // self-loop
        {0, 1, 4},    {-1, 1, 1}, // invalid
        {0, 2, 2},    {2, 99, 3}, // invalid
    };

    auto res = prim_mst(3, edges);
    EXPECT_EQ(res.components, 1);
    EXPECT_EQ(static_cast<int>(res.edges.size()), 2);
    EXPECT_EQ(res.total_weight, 2 + 4);
    EXPECT_TRUE(is_forest(3, res.edges));
}

TEST(PrimMst, NegativeWeightsAreHandled) {
    std::vector<Edge> edges = {
        {0, 1, -5},
        {1, 2, -1},
        {0, 2, 10},
    };

    auto res = prim_mst(3, edges);
    EXPECT_EQ(res.components, 1);
    EXPECT_EQ(res.total_weight, -6);
    EXPECT_TRUE(is_forest(3, res.edges));
}
