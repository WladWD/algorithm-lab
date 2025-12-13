# Barrier / Latch Implementations

Lock-free and low-contention **thread coordination primitives** for phase-based algorithms and concurrent systems.

This module provides:

- **`Barrier`** — a reusable rendezvous point for a fixed number of participating threads.
- **`CountDownLatch`** — a one-shot latch that allows one or more threads to wait until a counter reaches zero.

Both are implemented in modern C++ (C++20), using `std::atomic` and well-defined memory-ordering semantics.

---

## Motivation

In highly concurrent systems, threads often need to:

- **Synchronize at well-defined phases** of a computation (e.g., iterative solvers, parallel-for loops).
- **Wait for a set of tasks to complete** before proceeding (e.g., startup/shutdown, fan-out/fan-in patterns).

Traditional solutions rely on heavy-weight primitives (`std::mutex`, `std::condition_variable`) or ad-hoc spinning.
This module focuses on **lightweight, explicit synchronization**:

- Clear **happens-before** relationships between phases.
- Minimal shared state and cache contention.
- Friendly to **profiling**, **benchmarking**, and **teaching** the C++ memory model.

## Design & Memory Ordering

Both primitives are implemented using `std::atomic` with explicit **acquire/release** semantics.

### Barrier

- Maintains:
  - An **expected participant count**.
  - An **arrival counter** for the current phase.
  - A **phase epoch** (e.g., an incrementing generation number) to distinguish successive barrier rounds.
- On `arrive_and_wait()`:
  - Each thread **increments** the arrival counter using `fetch_add` with `memory_order_acq_rel`.
  - The **last arriving thread** resets the counter and **advances the phase**, publishing the new phase with `memory_order_release`.
  - Waiting threads **spin or block** until they observe the new phase with `memory_order_acquire`.

This ensures:

- All writes performed before `arrive_and_wait()` in any thread become **visible** to all threads after the barrier.
- The barrier does **not** reorder across the phase boundary.

### CountDownLatch

- Maintains an **atomic counter** initialized to `initial_count`.
- `count_down(n)` performs a `fetch_sub` with `memory_order_acq_rel`.
- When the counter reaches zero, waiting threads are woken up and observe completion with `memory_order_acquire`.

This ensures that:

- All work done by producers before the final `count_down()` is visible to consumers after `wait()` returns.

---

## Usage Examples

### Barrier: Phase-Based Parallel Loop

```cpp
#include "data_structures/lock_free/barrier/barrier.h"

#include <thread>
#include <vector>

void parallel_phase_example(std::size_t num_threads, std::size_t iterations) {
    Barrier barrier(num_threads);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (std::size_t tid = 0; tid < num_threads; ++tid) {
        threads.emplace_back([&, tid] {
            for (std::size_t iter = 0; iter < iterations; ++iter) {
                // Phase 1: local work
                // ... compute partial results ...

                barrier.arrive_and_wait();

                // Phase 2: reduce / consume results from other threads
                // ... read shared state updated by all threads ...

                barrier.arrive_and_wait();
            }
        });
    }

    for (auto &t : threads) {
        t.join();
    }
}
```

### CountDownLatch: Waiting for Workers

```cpp
#include "data_structures/lock_free/barrier/barrier.h"

#include <thread>
#include <vector>

void latch_example(std::size_t num_tasks) {
    CountDownLatch latch(num_tasks);
    std::vector<std::thread> threads;
    threads.reserve(num_tasks);

    for (std::size_t i = 0; i < num_tasks; ++i) {
        threads.emplace_back([&, i] {
            // Perform some work...
            // ...

            latch.count_down();
        });
    }

    // Wait for all tasks to complete
    latch.wait();

    for (auto &t : threads) {
        t.join();
    }
}
```
