# Dijkstra benchmarks — notes and expected profiling results

This document records expected benchmark results for the Dijkstra implementations in this directory and explains *why* those results are expected based on algorithmic characteristics and likely implementation trade-offs. Use this as a reference when interpreting benchmark runs and profiling results.

IMPORTANT: microbenchmark numbers are machine- and build-dependent. The results below come from a recent local run and are shown here as an example of expected relative behavior — absolute values will vary by CPU, OS, and compiler flags.

## Reported benchmark summary (reference run)
```
----------------------------------------------------------------------------------
Benchmark                                        Time             CPU   Iterations
----------------------------------------------------------------------------------
BM_DijkstraDijkstra_Sparse/100000       2758954750 ns   2758392000 ns            1
BM_DijkstraDijkstra_Queue_Sparse/100000     306619 ns       306571 ns         2373
BM_DijkstraDijkstra_Set_Sparse/100000      1341917 ns      1341369 ns          520
BM_DijkstraDijkstra_Dense/100000        9696496916 ns   9691912000 ns            1
BM_DijkstraDijkstra_Queue_Dense/100000   105187994 ns    105155571 ns            7
BM_DijkstraDijkstra_Set_Dense/100000     358432250 ns    358353500 ns            2
----------------------------------------------------------------------------------
```
The benchmarks above correspond to three different implementations exercised on two input regimes (sparse and dense). For clarity, the mapping of benchmark names to implementations used in this suite is unchanged:

- `Dijkstra_Sparse` — the N^2 regular (naive) implementation of Dijkstra (scans for minimum in O(N) per extraction).
- `Dijkstra_Queue_Sparse` — `std::priority_queue` (binary heap) based implementation that pushes duplicates on decrease-key.
- `Dijkstra_Set_Sparse` — `std::set` (ordered set) based implementation emulating decrease-key via erase+insert.

Below we interpret the new timings and explain plausible reasons for the observed ordering. 

## Interpretation & reasoning
- The `std::priority_queue` variant is the fastest on the *sparse* test (≈306 µs), followed by the `std::set` (≈1.34 ms). The N^2 naive variant is extremely slow on the same sparse workload (~2.76 s).
- On the *dense* input the `priority_queue` remains the fastest among these runs (≈0.105 s), the `std::set` is slower (≈0.358 s), and the N^2 naive variant is again catastrophic (~9.70 s).

## How to reproduce the exact run
1. Configure and build (use the repo presets):

```bash
cmake --preset dev
cmake --build --preset dev -j
```

2. Run only the dijkstra benchmarks (example):

```bash
./out/build/dev/benchmarks/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_min_time=2.0 --benchmark_repetitions=5
```