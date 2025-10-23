# Dijkstra benchmarks — notes and expected profiling results

This document records expected benchmark results for the Dijkstra implementations in this directory and explains *why* those results are expected based on algorithmic characteristics and likely implementation trade-offs. Use this as a reference when interpreting benchmark runs and profiling results.

IMPORTANT: microbenchmark numbers are machine- and build-dependent. The results below come from a recent local run (RelWithDebInfo / dev preset) and are shown here as an example of expected relative behavior — absolute values will vary by CPU, OS, and compiler flags.

## Reported benchmark summary (reference run)

----------------------------------------------------------------------------------
Benchmark                                        Time             CPU   Iterations
----------------------------------------------------------------------------------
BM_DijkstraDijkstra_Sparse/100000            33472 ns        33468 ns        20072
BM_DijkstraDijkstra_Queue_Sparse/100000     215316 ns       215272 ns         3255
BM_DijkstraDijkstra_Set_Sparse/100000         2911 ns         2910 ns       238064
BM_DijkstraDijkstra_Dense/100000             30795 ns        30791 ns        20225
BM_DijkstraDijkstra_Queue_Dense/100000   107639069 ns    107624333 ns            6
BM_DijkstraDijkstra_Set_Dense/100000        255772 ns       255704 ns         2731
----------------------------------------------------------------------------------

The benchmarks above correspond to three different implementations exercised on two input regimes (sparse and dense). For clarity, the mapping of benchmark names to implementations used in this suite is:

- `Dijkstra_Sparse` — the N^2 regular (naive) implementation of Dijkstra (scans for minimum in O(N) per extraction).
- `Dijkstra_Queue_Sparse` — `std::priority_queue` (binary heap) based implementation that pushes duplicates on decrease-key.
- `Dijkstra_Set_Sparse` — `std::set` (ordered set) based implementation emulating decrease-key via erase+insert.

Below we interpret the timings and explain plausible reasons for the observed ordering.

## Interpretation & reasoning

1) Overall pattern
- The run shows consistent relative behavior: on the *sparse* graph case the `Set` variant is the fastest (~2.9 µs), the N^2 naive implementation (`Dijkstra_Sparse`) is ~33.5 µs, and the `priority_queue` variant is ~215 µs.
- On the *dense* graph case: the N^2 naive implementation is ~30.8 µs (surprisingly comparable to sparse in this run), the `set`-variant slows to ~256 µs, and the `queue`-variant degrades catastrophically (~1.076e8 ns ≈ 0.108 s) with very few iterations, indicating pathological behavior on dense graphs.

2) Why the `set` variant can be fastest on sparse graphs
- An ordered `std::set` implementation that maintains a single current distance per vertex (erase + insert to emulate decrease-key) can avoid accumulating duplicate entries. On sparse graphs the number of relaxations is relatively small, so the set stays small and operations are efficient.
- This reduces memory traffic and the number of costly pop operations compared to an implementation that pushes many duplicate entries into the heap.

3) Why the N^2 regular implementation sits in the middle for these inputs
- The N^2 naive version scans all vertices to extract the minimum in O(N) per extraction. For sufficiently small effective N (or when the graph density and random weights lead to limited relaxations), this approach can be competitive due to very low per-operation overhead and excellent cache locality.
- In your run the N^2 variant is similar across sparse/dense regimes, which suggests the test parameters and graph generator lead to workloads where the scan-based extraction doesn't blow up asymptotically in observed sizes.

4) Why the `priority_queue` variant can be slow — and why it blows up on dense graphs
- The `std::priority_queue` implementation lacks decrease-key; each relaxation pushes a new pair (distance, vertex) into the heap. The heap can grow with many stale duplicates, making pop operations more expensive.
- In dense graphs the number of relaxations (updates) increases dramatically so the heap accumulates many duplicates; popping and discarding stale entries becomes dominant, which explains the catastrophic slowdown and the low iteration count in the benchmark for the dense queue run.

5) Implementation-level factors affecting these numbers
- Memory allocations and the size of containers during execution can dominate performance for some variants.
- Container traversal patterns (cache friendliness) favor simple arrays or tight loops (N^2 scan) over pointer-rich structures on modern CPUs.
- The `std::set` has logarithmic operations with relatively higher constants but may perform better when it keeps the active set small.

## Expected relationships (succinct)
- For the tested inputs we expect: `Dijkstra_Set` <= `Dijkstra_Sparse` <= `Dijkstra_Queue` on sparse graphs (where `<=` means "no slower than").
- On dense graphs the `Dijkstra_Queue` (priority_queue) may become much worse than the others due to many duplicate heap entries. The set-variant also slows (log V factor over many elements), while the N^2 naive variant can remain acceptable depending on V and cache effects.

## How to reproduce the exact run

1. Configure and build (use the repo presets):

```bash
cmake --preset dev
cmake --build --preset dev -j
```

2. Run only the dijkstra benchmarks (the binary path may differ depending on preset):

```bash
# Example path for the dev preset
./out/build/dev/benchmarks/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_min_time=2.0 --benchmark_repetitions=5
```

3. To collect machine-readable output (useful for CI or regression tracking):

```bash
./out/build/dev/benchmarks/algorithms/graphs/dijkstra/bench_graphs_dijkstra \
  --benchmark_min_time=2.0 --benchmark_repetitions=5 --benchmark_format=json > results.json
```