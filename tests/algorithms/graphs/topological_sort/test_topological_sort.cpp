#include <algorithms/graphs/topological_sort/topological_sort.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <vector>

namespace {
using algorithms::graphs::topological_sort::AdjList;
using algorithms::graphs::topological_sort::topological_order_or_empty;
using algorithms::graphs::topological_sort::topological_sort;

static bool is_topological_order_for_graph(const AdjList& g, const std::vector<int32_t>& order) {
    const int32_t n = static_cast<int32_t>(g.size());
    if ((int32_t)order.size() != n) {
        return false;
    }

    std::vector<int32_t> pos(static_cast<size_t>(n), -1);
    for (int32_t i = 0; i < n; ++i) {
        const int32_t v = order[static_cast<size_t>(i)];
        if (v < 0 || v >= n) {
            return false;
        }
        if (pos[static_cast<size_t>(v)] != -1) {
            return false; // duplicate
        }
        pos[static_cast<size_t>(v)] = i;
    }

    for (int32_t u = 0; u < n; ++u) {
        const auto su = static_cast<size_t>(u);
        for (int32_t v : g[su]) {
            if (v < 0 || v >= n) {
                continue; // invalid edges ignored by implementation
            }
            if (!(pos[static_cast<size_t>(u)] < pos[static_cast<size_t>(v)])) {
                return false;
            }
        }
    }

    return true;
}

TEST(TopologicalSort, EmptyGraph) {
    AdjList g;

    auto res = topological_sort(g);
    EXPECT_FALSE(res.has_cycle);
    EXPECT_TRUE(res.order.empty());

    EXPECT_TRUE(topological_order_or_empty(g).empty());
}

TEST(TopologicalSort, SingleVertex) {
    AdjList g(1);

    auto res = topological_sort(g);
    EXPECT_FALSE(res.has_cycle);
    ASSERT_EQ(res.order.size(), 1U);
    EXPECT_EQ(res.order[0], 0);
}

TEST(TopologicalSort, SimpleChainHasUniqueOrder) {
    AdjList g(4);
    // 0 -> 1 -> 2 -> 3
    g[0] = {1};
    g[1] = {2};
    g[2] = {3};

    auto res = topological_sort(g);
    EXPECT_FALSE(res.has_cycle);
    ASSERT_EQ(res.order.size(), 4U);

    // For a chain, the only valid topological order is [0,1,2,3].
    EXPECT_EQ(res.order, (std::vector<int32_t>{0, 1, 2, 3}));
}

TEST(TopologicalSort, MultipleValidOrdersValidateByPredicate) {
    AdjList g(4);
    // 0 -> 2, 1 -> 2, 2 -> 3; vertices 0 and 1 can be swapped.
    g[0] = {2};
    g[1] = {2};
    g[2] = {3};

    auto res = topological_sort(g);
    EXPECT_FALSE(res.has_cycle);
    ASSERT_TRUE(is_topological_order_for_graph(g, res.order));
}

TEST(TopologicalSort, DisconnectedDagCoversAllVertices) {
    AdjList g(6);
    // Component A: 0 -> 1
    g[0] = {1};
    // Component B: 2 -> 3
    g[2] = {3};
    // Component C: 4 isolated, 5 isolated

    auto res = topological_sort(g);
    EXPECT_FALSE(res.has_cycle);
    ASSERT_TRUE(is_topological_order_for_graph(g, res.order));
}

TEST(TopologicalSort, CycleIsDetected) {
    AdjList g(3);
    // 0 -> 1 -> 2 -> 0
    g[0] = {1};
    g[1] = {2};
    g[2] = {0};

    auto res = topological_sort(g);
    EXPECT_TRUE(res.has_cycle);
    EXPECT_LT(res.order.size(), 3U);

    EXPECT_TRUE(topological_order_or_empty(g).empty());
}

TEST(TopologicalSort, SelfLoopIsCycle) {
    AdjList g(2);
    g[0] = {0};

    auto res = topological_sort(g);
    EXPECT_TRUE(res.has_cycle);
    EXPECT_LT(res.order.size(), 2U);
}

TEST(TopologicalSort, IgnoresInvalidEdgesDefensively) {
    AdjList g(3);
    // Valid edge: 0 -> 1
    // Invalid edges: 1 -> -1, 1 -> 100
    g[0] = {1};
    g[1] = {-1, 100};

    auto res = topological_sort(g);
    EXPECT_FALSE(res.has_cycle);
    ASSERT_TRUE(is_topological_order_for_graph(g, res.order));
}

} // namespace

