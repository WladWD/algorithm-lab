# BUILD.md

> How to build, test, benchmark, profile, and enable sanitizers for this project.

## 0) Prerequisites

**macOS**
```bash
xcode-select --install          # Xcode command line tools
brew install cmake ninja        # recommended generator
```

**Linux (Ubuntu/Debian)**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake ninja-build
```

> CLion works out of the box with this setup (CMake + Ninja).

---

## 1) Configure & Build (with CMake Presets)

This repo uses `CMakePresets.json`, so you don’t need to remember flags.

### Dev (tests + benchmarks, `RelWithDebInfo`)
```bash
cmake --preset dev
cmake --build  --preset dev -j
```

### Release (benches only, optimized/LTO if configured)
```bash
cmake --preset release
cmake --build  --preset release -j
```

### Sanitizers (Debug + Address/Undefined)
```bash
cmake --preset sanitizers
cmake --build  --preset sanitizers -j
```

Each preset has its own build directory under `out/build/...`, so they don’t conflict.

---

## 2) Run Tests (GoogleTest)

Run all tests:
```bash
ctest --preset dev --output-on-failure
```

Run a single test binary:
```bash
./out/build/dev/algorithms/graphs/dijkstra/test_graphs_dijkstra
```

Filter by test name:
```bash
./out/build/dev/algorithms/graphs/dijkstra/test_graphs_dijkstra --gtest_filter=Dijkstra.*
```

---

## 3) Run Benchmarks (Google Benchmark)

Default run:
```bash
./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra
```

Stable, repeated run (recommended):
```bash
./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra   --benchmark_min_time=2.0   --benchmark_repetitions=10
```

Export JSON (for history/CI):
```bash
./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_format=json > results_dijkstra.json
```

**macOS tips for cleaner numbers**
- Close heavy apps; plug into AC power.
- Keep the machine cool (avoid thermal throttling).
- Prefer the median of several repetitions.

**Linux pinning example**
```bash
taskset -c 2-5 ./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra   --benchmark_min_time=2.0 --benchmark_repetitions=10   --benchmark_format=json > results_dijkstra.json
```

---

## 4) Profiling

### macOS (Instruments / from CLion)
- Open **Instruments → Time Profiler** and target the benchmark binary:
  ```text
  ./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_min_time=2.0
  ```
- Or in CLion: **Run → Profile ‘bench_graphs_dijkstra’**.

### Linux (perf + FlameGraph)
```bash
perf record -F 999 -g -- ./out/build/release/bench_graphs_dijkstra --benchmark_min_time=2.0
perf script | ./FlameGraph/stackcollapse-perf.pl | ./FlameGraph/flamegraph.pl > flame.svg
```

Suggested results layout:
```
results/
  machine-<host>/
    graphs/dijkstra/
      bench.json
      meta.json     # commit, CPU model, flags, etc.
      profile/      # flame.svg, perf.data, etc.
```

---

## 5) Sanitizers (ASan/UBSan)

Sanitizers are opt-in via CMake:

**Enable via preset**
```bash
cmake --preset sanitizers
cmake --build --preset sanitizers -j
ctest  --preset sanitizers
```

Notes:
- macOS supports **ASan** and **UBSan**. **TSan** isn’t supported on macOS; use Linux if you need data-race detection.
- Sanitizers slow programs down; don’t use them for final benchmark numbers.

---

## 6) CLion Integration

- CLion automatically detects **CMake targets**:
  - Tests → run via the built-in **Google Test** runner.
  - Benchmarks → add a “CMake Application” config for `bench_graphs_dijkstra`.
- To profile → **Run | Profile ‘bench_graphs_dijkstra’**.
- To enable sanitizers →
  Preferences → *Build, Execution, Deployment → Dynamic Analysis Tools → Sanitizers*.

---

## 7) Troubleshooting

- **“Ninja not found”** → `brew install ninja` or use `"generator": "Unix Makefiles"` in presets.
- **Include path errors** → confirm `target_include_directories(... PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)`.
- **Undefined references** → ensure tests/benches link with both `algo_graphs_dijkstra` and GTest/Benchmark libs.

---

Enjoy clean builds, measurable benchmarks, and safe memory checks!
