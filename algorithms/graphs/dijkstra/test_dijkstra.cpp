#include "dijkstra.h"
#include "generator.h"
#include <gtest/gtest.h>

TEST(Dijkstra, BasicCorrectness) {
    auto dist = algorithms::graphs::run();
    EXPECT_EQ(dist, 0);
    // add more invariants / known answers
}