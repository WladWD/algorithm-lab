#include "algorithms/graphs/bipartite_check/bipartite.h"

#include <gtest/gtest.h>

using namespace algorithms::graphs::bipartite_check;

static bool valid_coloring(const AdjList& g, const std::vector<int>& color) {
    if ((int)color.size() != (int)g.size())
        return false;
    for (int u = 0; u < (int)g.size(); ++u) {
        for (int v : g[u]) {
            if (color[u] == color[v])
                return false;
        }
    }
    return true;
}

TEST(BipartiteCheck, SimpleBipartite) {
    AdjList g(6);
    g[0] = {3, 4};
    g[1] = {3, 5};
    g[2] = {4, 5};
    g[3] = {0, 1};
    g[4] = {0, 2};
    g[5] = {1, 2};

    auto r1 = bipartite_bfs(g);
    ASSERT_TRUE(r1.has_value());
    EXPECT_TRUE(valid_coloring(g, *r1));

    auto r2 = bipartite_dfs(g);
    ASSERT_TRUE(r2.has_value());
    EXPECT_TRUE(valid_coloring(g, *r2));
}

TEST(BipartiteCheck, TriangleNotBipartite) {
    AdjList g(3);
    g[0] = {1, 2};
    g[1] = {0, 2};
    g[2] = {0, 1};

    EXPECT_FALSE(bipartite_bfs(g).has_value());
    EXPECT_FALSE(bipartite_dfs(g).has_value());
}

TEST(BipartiteCheck, DisconnectedGraph) {
    AdjList g(5);
    g[0] = {1};
    g[1] = {0};
    g[2] = {3};
    g[3] = {2};
    // node 4 isolated

    auto rb = bipartite_bfs(g);
    ASSERT_TRUE(rb.has_value());
    EXPECT_TRUE(valid_coloring(g, *rb));

    auto rd = bipartite_dfs(g);
    ASSERT_TRUE(rd.has_value());
    EXPECT_TRUE(valid_coloring(g, *rd));
}

TEST(BipartiteCheck, SelfLoop) {
    AdjList g(1);
    g[0] = {0};
    EXPECT_FALSE(bipartite_bfs(g).has_value());
    EXPECT_FALSE(bipartite_dfs(g).has_value());
}

TEST(BipartiteCheck, ParallelEdges) {
    AdjList g(2);
    g[0] = {1, 1, 1};
    g[1] = {0, 0, 0};
    auto rb = bipartite_bfs(g);
    ASSERT_TRUE(rb.has_value());
    EXPECT_TRUE(valid_coloring(g, *rb));
    auto rd = bipartite_dfs(g);
    ASSERT_TRUE(rd.has_value());
    EXPECT_TRUE(valid_coloring(g, *rd));
}

TEST(BipartiteCheck, RandomBipartiteAndNonBipartite) {
    const int n = 50;
    AdjList g(n);
    // build bipartite graph with partition [0..24] and [25..49]
    for (int u = 0; u < n / 2; ++u) {
        for (int v = n / 2; v < n; ++v) {
            if ((u + v) % 7 == 0) {
                g[u].push_back(v);
                g[v].push_back(u);
            }
        }
    }
    auto rb = bipartite_bfs(g);
    ASSERT_TRUE(rb.has_value());
    EXPECT_TRUE(valid_coloring(g, *rb));

    auto rd = bipartite_dfs(g);
    ASSERT_TRUE(rd.has_value());
    EXPECT_TRUE(valid_coloring(g, *rd));

    // add an odd cycle to break bipartiteness: pick three vertices a,b,c across partitions
    int a = 0, b = n / 2, c = 1; // 0 (left), n/2 (right), 1 (left) -> creates triangle
    g[a].push_back(b);
    g[b].push_back(a);
    g[b].push_back(c);
    g[c].push_back(b);
    g[c].push_back(a);
    g[a].push_back(c);

    EXPECT_FALSE(bipartite_bfs(g).has_value());
    EXPECT_FALSE(bipartite_dfs(g).has_value());
}
