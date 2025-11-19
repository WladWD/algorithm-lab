# Cache line alignment & prefetching

## Overview

Modern CPUs load memory in cache-line units (commonly 64 bytes). Aligning hot data structures to cache-line boundaries and organizing access patterns to exploit spatial locality can dramatically lower cache misses and improve throughput. Software prefetching lets you explicitly request the CPU fetch data into caches ahead of use, which can hide memory latency for predictable access patterns.

This document gives concise, practical recipes and code snippets you can drop into a C++ codebase.

---

## Cache-line basics

- Typical cache line size: 64 bytes on x86/x64; other architectures may use 64 or 128 bytes.
- A cache miss that requires fetching a line from a lower-level cache or DRAM is expensive (tens to hundreds of cycles).
- Useful metrics: L1/L2/L3 (LLC) cache miss rates, memory bandwidth, cycles per instruction (CPI).

Always check your target CPU's cache line size (use `lscpu`, `sysctl hw.cachelinesize`, or check architecture docs).

---

## Cache-line alignment: why it matters

- Avoid false sharing: two frequently-updated variables that share the same cache line on different threads cause cache-line ping-pong and slow down throughput.
- Improve streaming performance: when data fields used in a tight inner loop are contiguous and cache-line aligned, each fetched line holds many useful elements.
- Avoid unaligned loads/stores if the architecture penalizes them.

---

## Aligning data in C++ (examples)

Prefer portable APIs where possible. Examples below assume C++17/C++20.

1) Align a struct type to a cache-line boundary:

```cpp
// align struct to 64 bytes
struct alignas(64) AlignedCounter {
    alignas(64) std::atomic<int> value;
};
```

2) Allocate aligned memory for arrays (C++17/20):

```cpp
#include <cstdlib> // aligned_alloc on C11/C++17

size_t n = 1<<20;
size_t bytes = n * sizeof(float);
size_t alignment = 64; // cache-line alignment

// C11 aligned_alloc (alignment must divide size)
float* arr = static_cast<float*>(std::aligned_alloc(alignment, ((bytes + alignment -1)/alignment)*alignment));
if (!arr) { /* handle error */ }

// free with std::free(arr);
```

Notes:
- On POSIX systems you can use `posix_memalign(&ptr, alignment, size)` which returns 0 on success; alignment must be a power of two and multiple of `sizeof(void*)`.
- On Windows use `_aligned_malloc` / `_aligned_free`.

3) Use aligned containers or wrappers for RAII:

```cpp
#include <memory>
struct AlignedDeleter { void operator()(float* p) const { std::free(p); } };
std::unique_ptr<float, AlignedDeleter> arr_ptr(arr);
```

---

## Struct padding and formulas

To align an object to `A` bytes (power of two) you can compute padding as:

```text
padding = (A - (sizeof(T) % A)) % A
aligned_size = sizeof(T) + padding
```

Example in C++:

```cpp
constexpr size_t padding_for_align(size_t sz, size_t align) noexcept {
    return (align - (sz % align)) % align;
}
```

If you want to avoid false sharing for a per-thread counter, ensure each counter occupies its own cache line:

```cpp
struct CounterPadded {
    std::atomic<uint64_t> v;
    char pad[64 - sizeof(std::atomic<uint64_t>)]; // careful: use static_assert to ensure positive size
};
static_assert(sizeof(CounterPadded) == 64, "CounterPadded must be cache-line sized");
```

A safer approach is `alignas(64)` and rely on compiler padding:

```cpp
struct alignas(64) CounterAligned { std::atomic<uint64_t> v; };
```

---

## Software prefetching

Software prefetching requests the CPU to fetch a cache line into a specified cache level ahead of use. Use it for regular, predictable access patterns where memory latency hides behind useful work.

Portable GCC/Clang intrinsic:

```cpp
// read-prefetch
void prefetch_read(const void* ptr) {
    __builtin_prefetch(ptr, /*rw=*/0, /*locality=*/3);
}
// write-prefetch (less portable semantics)
void prefetch_write(void* ptr) {
    __builtin_prefetch(ptr, /*rw=*/1, /*locality=*/3);
}
```

Parameters:
- `rw`: 0 = read, 1 = write.
- `locality`: 0..3 (higher value keeps data in cache longer). Typical use: 1..2.

Example: prefetch in a loop with fixed stride

```cpp
for (size_t i = 0; i < n; ++i) {
    // prefetch element several iterations ahead
    size_t ahead = i + 16; // tune distance
    if (ahead < n) __builtin_prefetch(&arr[ahead], 0, 1);
    // do useful work on arr[i]
    process(arr[i]);
}
```

Heuristics for prefetch distance
- Prefetch distance depends on memory latency, iteration work per element, and CPU frequency.
- Start with a small distance (8..64) and experimentally tune: too small → no benefit, too large → pollutes caches.

Caution: modern CPUs have aggressive hardware prefetchers. Software prefetching can help when hardware prefetching fails (complex access patterns) but can hurt when used unnecessarily.

---

## Heuristics & best practices

- Profile before optimizing: use perf/VTune/Instruments to find memory-bound hotspots.
- Prefer data layout changes (SoA) or algorithmic fixes over hand-written prefetching when possible.
- Use `alignas(64)` for shared hot objects or per-thread counters to avoid false sharing.
- Use `std::aligned_alloc` / `posix_memalign` for aligned buffers. Wrap allocations in RAII.
- When prefetching:
  - Keep prefetch distance proportional to work between prefetch and use.
  - Limit prefetching to hot loops with predictable strides.
  - Benchmark with and without prefetching on the target hardware; results vary across CPUs.
