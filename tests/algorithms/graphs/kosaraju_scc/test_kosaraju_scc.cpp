#include <algorithm>
#include <algorithms/graphs/kosaraju_scc/kosaraju_scc.h>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace {
using algorithms::graphs::kosaraju_scc::AdjList;
using algorithms::graphs::kosaraju_scc::build_components;
using algorithms::graphs::kosaraju_scc::build_condensation_graph;
using algorithms::graphs::kosaraju_scc::kosaraju_scc;

static bool is_partition_of_vertices(const std::vector<std::vector<int32_t>>& comps, int32_t n) {
    std::vector<int32_t> seen(static_cast<size_t>(n), 0);
    int32_t cnt = 0;

    for (const auto& c : comps) {
        for (int32_t v : c) {
            if (v < 0 || v >= n) {
                return false;
            }
            if (seen[static_cast<size_t>(v)]) {
                return false;
            }
            seen[static_cast<size_t>(v)] = 1;
            cnt++;
        }
    }

    return cnt == n;
}

static bool is_dag(const AdjList& g) {
    const int32_t n = static_cast<int32_t>(g.size());
    std::vector<uint8_t> color(static_cast<size_t>(n), 0);

    // 0=white, 1=gray, 2=black
    std::function<bool(int32_t)> dfs = [&](int32_t v) {
        color[static_cast<size_t>(v)] = 1;
        for (int32_t to : g[static_cast<size_t>(v)]) {
            if (to < 0 || to >= n) {
                continue;
            }
            if (color[static_cast<size_t>(to)] == 1) {
                return false;
            }
            if (color[static_cast<size_t>(to)] == 0 && !dfs(to)) {
                return false;
            }
        }
        color[static_cast<size_t>(v)] = 2;
        return true;
    };

    for (int32_t v = 0; v < n; ++v) {
        if (color[static_cast<size_t>(v)] == 0) {
            if (!dfs(v)) {
                return false;
            }
        }
    }
    return true;
}

TEST(KosarajuScc, EmptyGraph) {
    AdjList g;

    auto scc = kosaraju_scc(g);
    EXPECT_TRUE(scc.component_of.empty());
    EXPECT_EQ(scc.components_count, 0);

    auto comps = build_components(scc);
    EXPECT_TRUE(comps.empty());

    auto dag = build_condensation_graph(g, scc);
    EXPECT_TRUE(dag.empty());
}

TEST(KosarajuScc, SingleVertexNoEdges) {
    AdjList g(1);

    auto scc = kosaraju_scc(g);
    ASSERT_EQ(scc.component_of.size(), 1U);
    EXPECT_EQ(scc.component_of[0], 0);
    EXPECT_EQ(scc.components_count, 1);

    auto comps = build_components(scc);
    ASSERT_EQ(comps.size(), 1U);
    EXPECT_EQ(comps[0], (std::vector<int32_t>{0}));
}

TEST(KosarajuScc, SimpleTwoCycleIsOneComponent) {
    AdjList g(2);
    g[0] = {1};
    g[1] = {0};

    auto scc = kosaraju_scc(g);
    EXPECT_EQ(scc.components_count, 1);
    EXPECT_EQ(scc.component_of[0], scc.component_of[1]);

    auto comps = build_components(scc);
    ASSERT_EQ(comps.size(), 1U);
    EXPECT_TRUE(is_partition_of_vertices(comps, 2));
}

TEST(KosarajuScc, TwoCyclesWithOneWayEdgeBecomeTwoComponents) {
    // SCC A: 0<->1, SCC B: 2<->3, edge A -> B.
    AdjList g(4);
    g[0] = {1, 2};
    g[1] = {0};
    g[2] = {3};
    g[3] = {2};

    auto scc = kosaraju_scc(g);
    EXPECT_EQ(scc.components_count, 2);
    EXPECT_EQ(scc.component_of[0], scc.component_of[1]);
    EXPECT_EQ(scc.component_of[2], scc.component_of[3]);
    EXPECT_NE(scc.component_of[0], scc.component_of[2]);
}

TEST(KosarajuScc, SelfLoopIsStillOneComponent) {
    AdjList g(2);
    g[0] = {0};

    auto scc = kosaraju_scc(g);
    EXPECT_EQ(scc.components_count, 2);
    EXPECT_EQ(scc.component_of[0], scc.component_of[0]);
    EXPECT_NE(scc.component_of[0], scc.component_of[1]);
}

TEST(KosarajuScc, DisconnectedGraphEachVertexOwnComponent) {
    AdjList g(5);

    auto scc = kosaraju_scc(g);
    EXPECT_EQ(scc.components_count, 5);

    auto comps = build_components(scc);
    ASSERT_EQ(static_cast<int32_t>(comps.size()), 5);
    EXPECT_TRUE(is_partition_of_vertices(comps, 5));
}

TEST(KosarajuScc, IgnoresInvalidEdgesDefensively) {
    AdjList g(3);
    g[0] = {1};
    g[1] = {-1, 100};
    g[2] = {2};

    auto scc = kosaraju_scc(g);
    EXPECT_EQ(scc.components_count, 3);

    auto dag = build_condensation_graph(g, scc);
    EXPECT_TRUE(is_dag(dag));
}

TEST(KosarajuScc, CondensationGraphIsDAGAndHasNoSelfLoops) {
    // SCC0: {0,1,2} cycle, SCC1: {3}, SCC2: {4,5}
    AdjList g(6);
    g[0] = {1};
    g[1] = {2};
    g[2] = {0, 3};
    g[3] = {4};
    g[4] = {5};
    g[5] = {4};

    auto scc = kosaraju_scc(g);
    auto dag = build_condensation_graph(g, scc);

    EXPECT_TRUE(is_dag(dag));

    for (int32_t u = 0; u < static_cast<int32_t>(dag.size()); ++u) {
        for (int32_t v : dag[static_cast<size_t>(u)]) {
            EXPECT_NE(u, v);
        }
    }
}

} // namespace
