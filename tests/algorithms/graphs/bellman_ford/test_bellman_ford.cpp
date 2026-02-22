#include "algorithms/graphs/bellman_ford/bellman_ford.h"

#include <gtest/gtest.h>

using namespace algorithms::graphs::bellman_ford;

namespace {

Graph make_clrs_demo_graph() {
    // Classic CLRS-style example (no negative cycles)
    // 0->1 (6), 0->2 (7)
    // 1->2 (8), 1->3 (5), 1->4 (-4)
    // 2->3 (-3), 2->4 (9)
    // 3->1 (-2)
    // 4->0 (2), 4->3 (7)
    Graph g(5);
    g[0] = {{1, 6}, {2, 7}};
    g[1] = {{2, 8}, {3, 5}, {4, -4}};
    g[2] = {{3, -3}, {4, 9}};
    g[3] = {{1, -2}};
    g[4] = {{0, 2}, {3, 7}};
    return g;
}

Graph make_negative_cycle_graph() {
    // 0 -> 1 (1)
    // 1 -> 2 (-1)
    // 2 -> 1 (-1)  => cycle weight = -2, reachable from 0
    Graph g(3);
    g[0] = {{1, 1}};
    g[1] = {{2, -1}};
    g[2] = {{1, -1}};
    return g;
}

} // namespace

TEST(BellmanFord, FindsShortestPathsWithNegativeEdges) {
    auto g = make_clrs_demo_graph();
    auto res = bellman_ford(g, 0);

    EXPECT_FALSE(res.has_negative_cycle);

    // Expected distances from CLRS example from source 0:
    // d = [0, 2, 7, 4, -2]
    ASSERT_EQ(res.dist.size(), 5u);
    EXPECT_EQ(res.dist[0], 0);
    EXPECT_EQ(res.dist[1], 2);
    EXPECT_EQ(res.dist[2], 7);
    EXPECT_EQ(res.dist[3], 4);
    EXPECT_EQ(res.dist[4], -2);

    // Parent sanity: reachable vertices other than source should have a parent.
    EXPECT_EQ(res.parent[0], -1);
    for (size_t v = 1; v < res.parent.size(); ++v) {
        EXPECT_NE(res.parent[v], -1);
    }
}

TEST(BellmanFord, DetectsReachableNegativeCycle) {
    auto g = make_negative_cycle_graph();
    auto res = bellman_ford(g, 0);
    EXPECT_TRUE(res.has_negative_cycle);
}

TEST(BellmanFord, OutOfRangeSourceReturnsEmpty) {
    Graph g(2);
    auto res = bellman_ford(g, 2);
    EXPECT_TRUE(res.dist.empty());
    EXPECT_TRUE(res.parent.empty());
    EXPECT_FALSE(res.has_negative_cycle);
}
