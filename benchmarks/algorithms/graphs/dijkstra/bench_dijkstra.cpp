#include "algorithms/graphs/dijkstra/dijkstra.h"

#include <benchmark/benchmark.h>
#include <functional>
#include <random>

using namespace algorithms::graphs::dijkstra;

namespace {
Graph make_worstcase_graph(int V, int E, unsigned seed) {
    Graph g(V);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> d(1, 1000000);

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

using Func = std::vector<int32_t> (*)(const Graph&, int32_t);
} // namespace

static void BM_Dijkstra(Func func, double ec, benchmark::State& st) {
    int V = (int)st.range(0);
    int E = V * (V - 1) / (8 + V * ec);
    auto g = make_worstcase_graph(V, E, 12345);
    for (auto _ : st) {
        auto d = func(g, 0);
        benchmark::DoNotOptimize(d);
    }
}

#define BM_Dijkstra(FUNC, NAME, EC)                                                                \
    static void BM_Dijkstra##NAME(benchmark::State& st) {                                          \
        BM_Dijkstra(FUNC, EC, st);                                                                 \
    }

#define RUN(FUNC, NAME, EC) BM_Dijkstra(FUNC, NAME, EC) BENCHMARK(BM_Dijkstra##NAME)->Arg(100000);

RUN(dijkstra, Dijkstra_Sparse, 0.9)
RUN(dijkstra_queue, Dijkstra_Queue_Sparse, 0.9)
RUN(dijkstra_set, Dijkstra_Set_Sparse, 0.9)

RUN(dijkstra, Dijkstra_Dense, 0.0001)
RUN(dijkstra_queue, Dijkstra_Queue_Dense, 0.0001)
RUN(dijkstra_set, Dijkstra_Set_Dense, 0.0001)

BENCHMARK_MAIN();