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

---

## API Overview

> **Note:** Names and exact signatures may evolve as the implementation matures. The intent is to stay close to the C++20 `std::barrier` / `std::latch` interfaces while keeping the code approachable.

### `Barrier`

Reusable barrier for a fixed number of participants.

```cpp
class Barrier {
public:
    explicit Barrier(std::size_t expected);

    // Arrive at the barrier and block until all participants have arrived.
    void arrive_and_wait();

    // Optional: arrive at the barrier but declare that this participant
    // will not take part in future phases (reduces the participant count).
    void arrive_and_drop();
};
```

**Key properties:**

- The barrier is **phase-based**. Each successful `arrive_and_wait()` advances the internal phase.
- All participating threads observe a **happens-before edge** from work done before the barrier to work done after it.
- Designed for **fixed-size thread pools** and iterative algorithms.

### `CountDownLatch`

One-shot latch for “wait until N events complete” scenarios.

```cpp
class CountDownLatch {
public:
    explicit CountDownLatch(std::size_t initial_count);

    // Decrement the counter by n (default 1). When the counter reaches zero,
    // all waiting threads are released.
    void count_down(std::size_t n = 1);

    // Block until the counter reaches zero.
    void wait();

    // Optional: timed wait that returns false on timeout.
    template <class Rep, class Period>
    bool wait_for(std::chrono::duration<Rep, Period> timeout);
};
```

**Typical use cases:**

- Main thread waits for a fixed number of worker tasks to finish.
- Coordinating startup/shutdown sequences.
- Testing and benchmarking concurrency scenarios.

---

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

> Implementation details (spinning vs. blocking, futex/condvar usage) are intentionally kept simple and explicit so that the code serves as a learning tool for the C++ memory model.

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

---

## Testing Strategy

The test suite for this module is expected to cover:

- **Basic correctness**
  - All threads can repeatedly synchronize on the same `Barrier` without deadlock.
  - `CountDownLatch` releases all waiters exactly once when the count reaches zero.
- **Ordering guarantees**
  - Work performed before the barrier/latch is visible after synchronization.
- **Edge cases**
  - Single-thread usage (trivial barrier/latch).
  - Large participant counts.
  - Stress tests with many iterations and randomized arrival patterns.

Running tests under **ThreadSanitizer** and **UndefinedBehaviorSanitizer** is recommended to validate the absence of data races and UB.

---

## Relationship to C++20 `std::barrier` / `std::latch`

The design of these primitives is intentionally close to the standard library equivalents:

- Similar conceptual model (fixed participant count, phase-based barrier, one-shot latch).
- Explicit **happens-before** guarantees.
- Emphasis on **teaching** and **experimentation** rather than replacing highly optimized standard library implementations.

This makes the code a good reference when learning:

- How to build higher-level synchronization primitives from `std::atomic`.
- How to reason about the C++ memory model in the presence of multiple threads.

---

## Directory Layout

Within the repository, this module is organized as follows:

- `include/data_structures/lock_free/barrier/barrier.h`
  - Public API for `Barrier` and `CountDownLatch`.
- `src/data_structures/lock_free/barrier/src/barrier.cpp`
  - Implementation details (atomics, phase management, waiting strategy).
- `tests/data_structures/lock_free/barrier/test_barrier.cpp`
  - Unit tests and stress tests for both primitives.

Refer to the top-level `README.md` and `docs/` for build, test, and benchmarking instructions.

