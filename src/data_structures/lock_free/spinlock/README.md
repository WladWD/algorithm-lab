# Spinlocks & Backoff Strategies

Lightweight **synchronization primitives** intended as a fallback under contention, and as educational building blocks for understanding lock-free and low-latency systems.

This module provides:

- **Spinlock** — a minimal lock built on top of `std::atomic_flag` / `std::atomic<bool>` for very short critical sections.
- **Backoff policies** — simple strategies for reducing contention (tight spinning, pause/yield, exponential backoff).

> These implementations are primarily for learning and benchmarking. In production code, prefer well-tested primitives from your standard library or concurrency framework unless you have a clear, measured need.

---

## Overview

Spinlocks are mutex-like primitives that **busy-wait** in a loop until they acquire the lock. They avoid kernel context switches but can increase CPU usage and cache contention if misused.

Backoff strategies control how a thread behaves while it is spinning:

- **Tight spin**: repeatedly poll the lock.
- **Pause/yield**: use CPU hints (e.g., `pause` on x86) or `std::this_thread::yield()` to reduce power and contention.
- **Exponential backoff**: progressively delay between attempts to avoid thundering herds.

This directory contains:

- A simple `Spinlock` implementation.
- A small set of backoff utilities that can be reused by other lock-free structures in this repository.

---

## API Sketch

```cpp
namespace data_structures::lock_free {

class Spinlock {
public:
    Spinlock() noexcept;

    void lock() noexcept;
    void unlock() noexcept;

    bool try_lock() noexcept;  // non-blocking attempt

private:
    std::atomic_flag flag_;
};

// Simple backoff helper suitable for use in spin loops.
class Backoff {
public:
    Backoff() noexcept;

    void pause() noexcept; // apply one step of backoff (spin/yield/sleep)

private:
    unsigned count_;
};

} // namespace data_structures::lock_free
```

The actual implementation may evolve, but this is the intended shape.

---

## Usage Example

```cpp
#include "data_structures/lock_free/spinlock/spinlock.h"

#include <thread>
#include <vector>

using data_structures::lock_free::Spinlock;

int main() {
    Spinlock lock;
    int shared_counter = 0;

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < 100000; ++j) {
                lock.lock();
                ++shared_counter;
                lock.unlock();
            }
        });
    }

    for (auto &t : threads) {
        t.join();
    }

    // shared_counter should be 4 * 100000
}
```

---

## Notes & Caveats

- Spinlocks are appropriate **only for very short critical sections** and when threads are expected to hold the lock for a tiny amount of time.
- Under high contention or long critical sections, spinlocks can waste CPU cycles and perform worse than blocking mutexes.
- These implementations are intentionally kept simple and explicit for teaching and experimentation; they are not drop-in replacements for production-grade synchronization primitives.

Refer to the top-level `README.md` and `docs/` for build and testing instructions.

