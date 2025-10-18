#include "algorithms/graphs/dijkstra/dijkstra.h"

#include <benchmark/benchmark.h>
#include <random>

using namespace algorithms::graphs::dijkstra;

namespace {
Graph make_worstcase_graph(int V, int E, unsigned seed) {
    Graph                              g(V);
    std::mt19937                       rng(seed);
    std::uniform_int_distribution<int> d(1, 9);

    for (int i = 0; i + 1 < V; ++i)
        g[i].push_back({i + 1, d(rng)});
    for (int i = 0; i < E - (V - 1); ++i) {
        int u = rng() % V, v = rng() % V;
        if (u == v)
            v = (v + 1) % V;
        g[u].push_back({v, d(rng)});
    }
    return g;
}
} // namespace

static void BM_Dijkstra(benchmark::State& st) {
    int  V = (int)st.range(0);
    int  E = V * (V - 1) / 8;
    auto g = make_worstcase_graph(V, E, 12345);
    for (auto _ : st) {
        auto d = dijkstra(g, 0);
        benchmark::DoNotOptimize(d);
    }
}
BENCHMARK(BM_Dijkstra)->Arg(2000);
BENCHMARK_MAIN();