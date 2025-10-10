#include "algorithms/graphs/dijkstra/dijkstra.h"
#include "algorithms/graphs/dijkstra/generator.h"
#include <benchmark/benchmark.h>

using namespace algorithms::graphs::dijkstra;

static void BM_Dijkstra(benchmark::State& st) {
  int V = (int)st.range(0);
  int E = V * (V - 1) / 8;
  auto g = make_worstcase_graph(V, E, 12345);
  for (auto _ : st) {
    auto d = dijkstra(g, 0);
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Dijkstra)->Arg(2000);
BENCHMARK_MAIN();