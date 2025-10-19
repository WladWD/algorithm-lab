# Profiling & Google Benchmark — Algorithm Lab

This document explains how to run Google Benchmark binaries, collect profiling data, and produce flamegraphs where applicable. It covers macOS and Linux notes.

Build for benchmarking
- Use a build preset that enables benchmarks (the `dev` and `release` presets enable benchmarks per `CMakePresets.json`). `dev` uses `RelWithDebInfo` which is a good compromise for profiling.

```bash
cmake --preset dev
cmake --build --preset dev -j
```

Running Google Benchmark binaries
- Benchmarks live under `out/build/<preset>/...`.

Example:

```bash
# Run a benchmark with a minimum time per benchmark
./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_min_time=2.0

# Filter benchmarks by name (regex)
./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_filter=.*Dijkstra.*

# Run N repetitions and print full statistics
./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_repetitions=5 --benchmark_min_time=1.0
```

Benchmark flags reference (use `--help` on the binary for full list):
- `--benchmark_min_time` — minimum wall time per benchmark
- `--benchmark_repetitions` — number of independent repetitions
- `--benchmark_filter` — regex to select benchmarks
- `--benchmark_format=json` — output machine-readable results for CI/postprocessing

Collecting CPU profiles

macOS (recommended tooling)
- Use Instruments (Time Profiler) for a GUI-based sampling profiler.
  - Open Instruments, choose Time Profiler, run the benchmark binary, and inspect stacks.
- For a quick CLI sampling: use `sample` (macOS built-in):

```bash
# Start the benchmark in background and sample it (example: 10 seconds)
./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_min_time=2.0 &
PID=$!
sample $PID 5 -file sample.txt
# Inspect sample.txt for hotspots
```

Linux (perf / flamegraphs)
- On Linux, `perf` + FlameGraph scripts are a standard approach:

```bash
# Record with perf (must have `perf` and kernel with frame pointers)
perf record -F 99 -g -- ./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_min_time=2.0
perf script > out/perf.unfold
# Convert to folded stacks and produce a flamegraph (FlameGraph scripts required)
stackcollapse-perf.pl out/perf.unfold > out/perf.folded
flamegraph.pl out/perf.folded > out/flamegraph.svg
```

Generating profiles suitable for flamegraphs requires building with frame pointers or using DWARF-based unwinding. Consider `-fno-omit-frame-pointer` in the build flags if stack traces are incomplete.

Memory profiling
- macOS: use Instruments (Allocations/Leaks) to inspect memory usage
- Linux: use `massif` (Valgrind) or heap-profilers (google-heap-profiler / tcmalloc) if available

Collecting benchmark output for CI
- Google Benchmark can emit JSON with `--benchmark_format=json`. Collect and upload that file in CI to preserve results.

Best practices
- Use `RelWithDebInfo` or `Release` for realistic performance numbers (avoid `Debug` where compiler optimizations are disabled).
- Repeat benchmarks (`--benchmark_repetitions`) and use `--benchmark_min_time` to reduce noise.
- Pin CPU frequency scaling (on Linux) and disable turbo boost for stable results when comparing microseconds-level numbers.
- For deep investigation, combine Google Benchmark with a sampling profiler and a flamegraph to find hotspots.

# Debugging — Algorithm Lab

This guide explains how to build and run the project for debugging, including sanitizer usage and running under a debugger on macOS (similar steps apply on Linux).

Build modes for debugging
- Use the `sanitizers` preset to enable Address/Undefined Behavior sanitizers and a Debug build configuration:

```bash
cmake --preset sanitizers
cmake --build --preset sanitizers -j
```

This produces build artifacts under `out/build/sanitizers`.

AddressSanitizer / UndefinedBehaviorSanitizer
- ASan/UBSan are very useful to catch memory errors and undefined behavior early. Useful environment variables:
  - ASAN_OPTIONS=verbosity=1:halt_on_error=1:detect_leaks=1
  - UBSAN_OPTIONS=print_stacktrace=1

Example: run a test binary with ASAN options:

```bash
ASAN_OPTIONS=verbosity=1:halt_on_error=1:detect_leaks=1 \
  ./out/build/sanitizers/algorithms/graphs/dijkstra/test_graphs_dijkstra
```

Running under lldb (macOS)

```bash
# Start lldb with the binary
lldb -- ./out/build/sanitizers/algorithms/graphs/dijkstra/test_graphs_dijkstra
# Inside lldb, run
run
# If the binary crashes, use `bt` to get a backtrace
bt
```

Running tests under the debugger
- Instead of `ctest`, run individual test binaries under the debugger. To find test binaries, inspect `out/build/<preset>/` or use `ctest -N` to list tests.

Debugging tips
- Build with debug symbols (Debug or RelWithDebInfo) — the provided `dev` preset uses `RelWithDebInfo` which includes symbols and optimizations.
- If ASan reports errors with inlined stack frames, set `ASAN_SYMBOLIZER_PATH` to a compatible symbolizer (typically installed with LLVM/Clang tools).
- For race conditions, consider using ThreadSanitizer (TSan) — may require a separate preset or toolchain flag.
- Keep repro steps minimal: run the failing test or a small harness repeatedly under the sanitizer/debugger.

Suppressions
- If you need to suppress known leaks or false positives, provide an ASan suppression file and pass it with `ASAN_OPTIONS=suppressions=/path/to/suppressions.txt`.

IDE support
- CLion and many IDEs understand CMake presets and can run the `sanitizers` preset configuration and attach the debugger.
- For VSCode, point the C++ extension to the compile commands file in the build dir and configure launch.json to use the binaries under `out/build/<preset>`.

Common commands summary

```bash
# Configure sanitizer build
cmake --preset sanitizers
cmake --build --preset sanitizers -j

# Run a single test binary under lldb
lldb -- ./out/build/sanitizers/path/to/test_binary

# Run with ASan options (run normally)
ASAN_OPTIONS=verbosity=1:halt_on_error=1:detect_leaks=1 ./out/build/sanitizers/path/to/test_binary
```

