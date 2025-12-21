#include <algorithms/graphs/dfs/dfs.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <unordered_set>
#include <vector>

namespace {
using algorithms::graphs::dfs::AdjList;
using algorithms::graphs::dfs::dfs_forest;
using algorithms::graphs::dfs::dfs_forest_recursive;
using algorithms::graphs::dfs::dfs_order_from;
using algorithms::graphs::dfs::dfs_order_from_recursive;

static bool is_permutation_0n1(const std::vector<int32_t>& order, int32_t n) {
    if ((int32_t)order.size() != n) {
        return false;
    }

    std::vector<int32_t> cnt(static_cast<size_t>(n), 0);
    for (int32_t v : order) {
        if (v < 0 || v >= n) {
            return false;
        }
        cnt[static_cast<size_t>(v)]++;
    }
    return std::all_of(cnt.begin(), cnt.end(), [](int32_t x) { return x == 1; });
}

TEST(DFS, EmptyGraph) {
    AdjList g;

    auto res = dfs_forest(g);
    EXPECT_TRUE(res.order.empty());
    EXPECT_TRUE(res.parent.empty());
    EXPECT_TRUE(res.tin.empty());
    EXPECT_TRUE(res.tout.empty());

    EXPECT_TRUE(dfs_order_from(g, 0).empty());
}

TEST(DFS, SingleVertex) {
    AdjList g(1);

    auto res = dfs_forest(g);
    ASSERT_EQ(res.order.size(), 1U);
    EXPECT_EQ(res.order[0], 0);
    ASSERT_EQ(res.parent.size(), 1U);
    EXPECT_EQ(res.parent[0], -1);

    ASSERT_EQ(res.tin.size(), 1U);
    ASSERT_EQ(res.tout.size(), 1U);
    EXPECT_GE(res.tin[0], 0);
    EXPECT_GT(res.tout[0], res.tin[0]);
}

TEST(DFS, DisconnectedForestCoversAllVertices) {
    AdjList g(6);
    // Component 1: 0-1-2
    g[0] = {1};
    g[1] = {0, 2};
    g[2] = {1};
    // Component 2: 3-4
    g[3] = {4};
    g[4] = {3};
    // Vertex 5 isolated.

    auto res = dfs_forest(g);
    ASSERT_TRUE(is_permutation_0n1(res.order, 6));
    ASSERT_EQ(res.parent.size(), 6U);

    // Roots must have parent = -1.
    int roots = 0;
    for (int v = 0; v < 6; ++v) {
        if (res.parent[static_cast<size_t>(v)] == -1) {
            roots++;
        }
    }
    // We expect 3 roots: one per component.
    EXPECT_EQ(roots, 3);

    // Timestamps are assigned for all vertices.
    for (int v = 0; v < 6; ++v) {
        EXPECT_NE(res.tin[static_cast<size_t>(v)], -1);
        EXPECT_NE(res.tout[static_cast<size_t>(v)], -1);
        EXPECT_LT(res.tin[static_cast<size_t>(v)], res.tout[static_cast<size_t>(v)]);
    }
}

TEST(DFS, FromStartVisitsReachableOnly) {
    AdjList g(5);
    // 0 -> 1 -> 2, and 3 -> 4
    g[0] = {1};
    g[1] = {2};
    g[3] = {4};

    auto order0 = dfs_order_from(g, 0);
    ASSERT_EQ(order0.size(), 3U);
    EXPECT_EQ(order0[0], 0);

    std::unordered_set<int32_t> s(order0.begin(), order0.end());
    EXPECT_TRUE(s.count(0));
    EXPECT_TRUE(s.count(1));
    EXPECT_TRUE(s.count(2));
    EXPECT_FALSE(s.count(3));
    EXPECT_FALSE(s.count(4));

    EXPECT_TRUE(dfs_order_from(g, -1).empty());
    EXPECT_TRUE(dfs_order_from(g, 5).empty());
}

TEST(DFS, TimestampsHaveProperNestingForTreeEdges) {
    AdjList g(4);
    // Undirected chain 0-1-2-3
    g[0] = {1};
    g[1] = {0, 2};
    g[2] = {1, 3};
    g[3] = {2};

    auto res = dfs_forest(g);

    for (int v = 0; v < 4; ++v) {
        const int p = res.parent[static_cast<size_t>(v)];
        if (p == -1) {
            continue;
        }

        // For a parent-child relation in DFS tree, intervals must be nested:
        // tin[p] < tin[v] < tout[v] < tout[p].
        EXPECT_LT(res.tin[static_cast<size_t>(p)], res.tin[static_cast<size_t>(v)]);
        EXPECT_LT(res.tin[static_cast<size_t>(v)], res.tout[static_cast<size_t>(v)]);
        EXPECT_LT(res.tout[static_cast<size_t>(v)], res.tout[static_cast<size_t>(p)]);
    }
}

TEST(DFS, RecursiveEmptyGraph) {
    AdjList g;

    auto res = dfs_forest_recursive(g);
    EXPECT_TRUE(res.order.empty());
    EXPECT_TRUE(res.parent.empty());
    EXPECT_TRUE(res.tin.empty());
    EXPECT_TRUE(res.tout.empty());

    EXPECT_TRUE(dfs_order_from_recursive(g, 0).empty());
}

TEST(DFS, RecursiveDisconnectedForestCoversAllVertices) {
    AdjList g(6);
    // Component 1: 0-1-2
    g[0] = {1};
    g[1] = {0, 2};
    g[2] = {1};
    // Component 2: 3-4
    g[3] = {4};
    g[4] = {3};
    // Vertex 5 isolated.

    auto res = dfs_forest_recursive(g);
    ASSERT_TRUE(is_permutation_0n1(res.order, 6));

    int roots = 0;
    for (int v = 0; v < 6; ++v) {
        if (res.parent[static_cast<size_t>(v)] == -1) {
            roots++;
        }
    }
    EXPECT_EQ(roots, 3);

    for (int v = 0; v < 6; ++v) {
        EXPECT_NE(res.tin[static_cast<size_t>(v)], -1);
        EXPECT_NE(res.tout[static_cast<size_t>(v)], -1);
        EXPECT_LT(res.tin[static_cast<size_t>(v)], res.tout[static_cast<size_t>(v)]);
    }
}

TEST(DFS, RecursiveFromStartVisitsReachableOnly) {
    AdjList g(5);
    // 0 -> 1 -> 2, and 3 -> 4
    g[0] = {1};
    g[1] = {2};
    g[3] = {4};

    auto order0 = dfs_order_from_recursive(g, 0);
    ASSERT_EQ(order0.size(), 3U);
    EXPECT_EQ(order0[0], 0);

    std::unordered_set<int32_t> s(order0.begin(), order0.end());
    EXPECT_TRUE(s.count(0));
    EXPECT_TRUE(s.count(1));
    EXPECT_TRUE(s.count(2));
    EXPECT_FALSE(s.count(3));
    EXPECT_FALSE(s.count(4));

    EXPECT_TRUE(dfs_order_from_recursive(g, -1).empty());
    EXPECT_TRUE(dfs_order_from_recursive(g, 5).empty());
}

TEST(DFS, RecursiveAndIterativeReachSameSetFromStart) {
    AdjList g(6);
    g[0] = {1, 2};
    g[1] = {3};
    g[2] = {3, 4};
    g[3] = {5};

    auto it = dfs_order_from(g, 0);
    auto rec = dfs_order_from_recursive(g, 0);

    std::unordered_set<int32_t> sit(it.begin(), it.end());
    std::unordered_set<int32_t> srec(rec.begin(), rec.end());
    EXPECT_EQ(sit, srec);
}

} // namespace

