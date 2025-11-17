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
- SoA: accessing a field is stride-1; several consecutive field values fit in the same cache line, maximizing useful bytes per line.

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



