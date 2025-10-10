#pragma once

#include <random>

#include "algorithms/graphs/dijkstra/dijkstra.h"

namespace algorithms::graphs::dijkstra {
Graph make_worstcase_graph(int V, int E, unsigned seed = 12345);
Graph make_demo_graph();
} // namespace algorithms::graphs::dijkstra