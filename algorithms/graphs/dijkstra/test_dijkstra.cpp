#include "algorithms/graphs/dijkstra/dijkstra.h"
#include "algorithms/graphs/dijkstra/generator.h"
#include <gtest/gtest.h>

using namespace algorithms::graphs::dijkstra;

TEST(Dijkstra, Demo) {
  auto g = make_demo_graph();
  auto d = dijkstra(g, 0);
  EXPECT_EQ(d[0], 0);
  EXPECT_EQ(d[3], 3);
}