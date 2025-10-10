#include "dijkstra.h"
#include "generator.h"
#include <benchmark/benchmark.h>

static void BM_Dijkstra_Dense(benchmark::State& state) {
    // Fixed-size worst case (deterministic)
    //const int V = static_cast<int>(state.range(0));
    //const int E = V * (V - 1) / 4; // e.g., quarter dense; tune as needed
    //auto g = make_worstcase_graph(V, E, /*seed=*/12345);

    for (auto _ : state) {
        auto dist = algorithms::graphs::run();//dijkstra(g, /*source=*/0);
        benchmark::DoNotOptimize(dist);
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));
}

BENCHMARK(BM_Dijkstra_Dense)->Arg(1'000)->Unit(benchmark::kMillisecond);
BENCHMARK_MAIN();