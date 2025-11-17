# AoS vs SoA (memory layout)

This benchmark explores two common memory layouts used in systems and numerical code:

- AoS — Array of Structs: store objects as a contiguous array of structs (object-centric layout).
- SoA — Struct of Arrays: store each field in a separate contiguous array (field-centric layout).

## Overview

AoS packages all fields of an object together. SoA places each field in its own contiguous array. The choice affects cache usage, prefetching, and vectorization.

- AoS: good when code accesses most/all fields of one object together.
- SoA: good when code streams over one or a few fields across many objects (data-parallel workloads).

## When to use AoS vs SoA

Use AoS when:
- Code naturally manipulates whole objects.
- Access patterns are per-object (all fields used together).
- Simplicity and API convenience are higher priority.

Use SoA when:
- You perform tight loops operating on one or two fields across many objects.
- You need contiguous memory for a field to enable SIMD or efficient prefetching.

## Memory behavior (intuitive)

Let `L` be cache line size (bytes), `S` be struct size (bytes). If you access only one field of size `f` inside the struct:

- AoS: each accessed field may reside on a cache line together with other unused fields. The effective useful bytes per cache line is ~f, so bandwidth is wasted.
- SoA: several consecutive field values fit in the same cache line, maximizing useful bytes per line.

Therefore SoA typically has better bandwidth utilization for single-field streaming workloads.

## Example C++ code

Below are minimal AoS and SoA representations used by the benchmark. See `main.cpp` in this directory for the full profiling harness.

AoS example:

```cpp
struct ParticleAoS {
    float x, y, z;   // position
    float vx, vy, vz; // velocity
};
std::vector<ParticleAoS> v(n);
```

SoA example:

```cpp
struct ParticlesSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    ParticlesSoA(size_t n) : x(n), y(n), z(n), vx(n), vy(n), vz(n) {}
};
ParticlesSoA s(n);
```

## Notes

- Results depend on CPU architecture, compiler, optimization flags, and data size relative to caches.
- Always profile real workloads: synthetic benchmarks are guides, not definitive answers.

## Analyzing benchmark output

Below is a short guide to read and interpret the Google Benchmark output you pasted. The benchmark prints per-benchmark entries such as:

- Benchmark name / problem size (e.g. `BM_SumX_AoS/10000000`)
- Time / CPU / Iterations
- UserCounters (we set `items_per_second`) — throughput reported as items/sec

Example summary lines from your run (selected rows):

```
BM_SumX_AoS/10000000         5866546 ns      5864798 ns          124 items_per_second=1.70509G/s
BM_SumX_SoA/10000000         4907574 ns      4903490 ns          145 items_per_second=2.03936G/s
BM_UpdateX_AoS/10000000      3561767 ns      3561597 ns          196 items_per_second=2.80773G/s
BM_UpdateX_SoA/10000000       811268 ns       810949 ns          864 items_per_second=12.3312G/s
BM_UpdateAll_AoS/10000000    5851310 ns      5850508 ns          120 items_per_second=5.12776G/s
BM_UpdateAll_SoA/10000000    4330691 ns      4330037 ns          162 items_per_second=6.92835G/s
```

Quick numeric takeaways (speedups = SoA / AoS):

```
sum_x (10M):     2.03936 / 1.70509 = 1.20x  (SoA ~20% faster)
update_x (10M): 12.3312 / 2.80773 = 4.39x  (SoA ~4.4× faster)
update_all (10M):6.92835 / 5.12776 = 1.35x  (SoA ~35% faster)
```

How to read these numbers

- `items_per_second` = N / elapsed_time. Higher is better. Use it to compare throughput between layouts.
- `Time` (ns) is wall-clock time for each benchmark invocation (the benchmark handles multiple iterations and statistics).
- Check iterations: many iterations reduce noise; Google Benchmark reports statistical aggregates.

Interpretation (why SoA often wins here)

- Single-field streaming (`sum_x` or `update_x`): SoA stores the `x` (and `vx`) arrays contiguously. This gives stride-1 accesses and much better cache-line utilization and prefetching. That explains the large win for `update_x` (which reads `vx` and writes `x`) and a consistent advantage for `sum_x`.

- Multi-field updates (`update_all`): AoS groups fields per object which gives locality when you access many fields of the same object. However, in this run SoA still outperforms AoS (35% for 10M). This can happen for two reasons:
  - The CPU's memory subsystem (prefetcher, store-combining) and vectorization may favor contiguous per-field streams even when you touch several fields.
  - Write-allocations and cache eviction patterns depend on struct layout and size; SoA sometimes reduces cache conflicts.

In summary, SoA often outperforms AoS for single-field streaming workloads due to better cache utilization and prefetching. For multi-field access, the results can vary, but SoA can still have advantages depending on access patterns and CPU architecture.