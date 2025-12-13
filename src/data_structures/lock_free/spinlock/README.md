# Spinlocks & Backoff Strategies

Lightweight **synchronization primitives** intended as a fallback under contention, and as educational building blocks for understanding lock-free and low-latency systems.

This module provides:

- **Spinlock** — a minimal lock built on top of `std::atomic_flag` / `std::atomic<bool>` for very short critical sections.
- **Backoff policies** — simple strategies for reducing contention (tight spinning, pause/yield, exponential backoff).

---

## Overview

Spinlocks are mutex-like primitives that **busy-wait** in a loop until they acquire the lock. They avoid kernel context switches but can increase CPU usage and cache contention if misused.

Backoff strategies control how a thread behaves while it is spinning:

- **Tight spin**: repeatedly poll the lock.
- **Pause/yield**: use CPU hints (e.g., `pause` on x86) or `std::this_thread::yield()` to reduce power and contention.
- **Exponential backoff**: progressively delay between attempts to avoid thundering herds.

## Notes & Caveats

- Spinlocks are appropriate **only for very short critical sections** and when threads are expected to hold the lock for a tiny amount of time.
- Under high contention or long critical sections, spinlocks can waste CPU cycles and perform worse than blocking mutexes.
- These implementations are intentionally kept simple and explicit for teaching and experimentation; they are not drop-in replacements for production-grade synchronization primitives.


