#include <algorithms/graphs/floyd_warshall/floyd_warshall.h>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace fw = algorithms::graphs::floyd_warshall;

TEST(FloydWarshall, ComputesAllPairsShortestPaths) {
    const int32_t INF = fw::FloydWarshallResult::INF;

    // 0 -> 1 (5)
    // 0 -> 2 (10)
    // 1 -> 2 (3)
    std::vector<std::vector<int32_t>> w = {
        {0, 5, 10},
        {INF, 0, 3},
        {INF, INF, 0},
    };

    auto res = fw::floyd_warshall(w);

    ASSERT_FALSE(res.has_negative_cycle);
    ASSERT_EQ(res.dist.size(), 3u);

    EXPECT_EQ(res.dist[0][0], 0);
    EXPECT_EQ(res.dist[0][1], 5);
    EXPECT_EQ(res.dist[0][2], 8); // 0->1->2

    EXPECT_EQ(res.dist[1][0], INF);
    EXPECT_EQ(res.dist[1][1], 0);
    EXPECT_EQ(res.dist[1][2], 3);

    EXPECT_EQ(res.dist[2][0], INF);
    EXPECT_EQ(res.dist[2][1], INF);
    EXPECT_EQ(res.dist[2][2], 0);

    // Path reconstruction sanity: 0 -> 2 should start with 1.
    ASSERT_EQ(res.next[0][2], 1);
    ASSERT_EQ(res.next[0][1], 1);
}

TEST(FloydWarshall, HandlesNegativeEdgesWithoutNegativeCycle) {
    const int32_t INF = fw::FloydWarshallResult::INF;

    // 0 -> 1 (1)
    // 1 -> 2 (-2)
    // 0 -> 2 (4)
    std::vector<std::vector<int32_t>> w = {
        {0, 1, 4},
        {INF, 0, -2},
        {INF, INF, 0},
    };

    auto res = fw::floyd_warshall(w);

    ASSERT_FALSE(res.has_negative_cycle);
    EXPECT_EQ(res.dist[0][2], -1); // 0->1->2 = 1 + (-2)
}

TEST(FloydWarshall, DetectsNegativeCycle) {
    const int32_t INF = fw::FloydWarshallResult::INF;

    // Cycle 0 -> 1 -> 2 -> 0 with total weight -1.
    std::vector<std::vector<int32_t>> w = {
        {0, 1, INF},
        {INF, 0, -3},
        {1, INF, 0},
    };

    auto res = fw::floyd_warshall(w);

    ASSERT_TRUE(res.has_negative_cycle);
    EXPECT_TRUE(res.dist[0][0] < 0 || res.dist[1][1] < 0 || res.dist[2][2] < 0);
}

TEST(FloydWarshall, NonSquareMatrixReturnsEmpty) {
    std::vector<std::vector<int32_t>> w = {
        {0, 1},
        {2, 3},
        {4, 5},
    };

    auto res = fw::floyd_warshall(w);
    EXPECT_TRUE(res.dist.empty());
    EXPECT_TRUE(res.next.empty());
    EXPECT_FALSE(res.has_negative_cycle);
}
