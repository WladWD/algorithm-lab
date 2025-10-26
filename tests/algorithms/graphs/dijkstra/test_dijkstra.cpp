#include "algorithms/graphs/dijkstra/dijkstra.h"

#include <gtest/gtest.h>
#include <random>

using namespace algorithms::graphs::dijkstra;

namespace {
Graph make_demo_graph() {
    Graph g(4);
    g[0] = {{1, 1}, {2, 5}};
    g[1] = {{2, 1}, {3, 2}};
    g[2] = {{3, 1}};
    return g;
}
} // namespace

TEST(Dijkstra, Demo) {
    auto g = make_demo_graph();
    auto d = dijkstra(g, 0);
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[3], 3);

    auto d_set = dijkstra_set(g, 0);
    EXPECT_EQ(d_set[0], 0);
    EXPECT_EQ(d_set[3], 3);

    auto d_queue = dijkstra_queue(g, 0);
    EXPECT_EQ(d_queue[0], 0);
    EXPECT_EQ(d_queue[3], 3);
}