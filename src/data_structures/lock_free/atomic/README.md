# C++ Atomics & Memory Ordering Cheat Sheet

A comprehensive reference guide for understanding `std::atomic` and the C++ Memory Model. This document serves as a guide for developers transitioning from mutex-based synchronization to lock-free programming.

## 📋 Table of Contents

- [Introduction](#introduction)
- [Atomic Basics](#atomic-basics)
- [Memory Ordering Models](#memory-ordering-models)
    - [Sequential Consistency (`seq_cst`)](#1-sequential-consistency-seq_cst)
    - [Acquire-Release (`acquire`, `release`)](#2-acquire-release)
    - [Relaxed (`relaxed`)](#3-relaxed)
- [Comparison Table](#ordering-comparison)
- [Code Examples](#code-examples)
- [Common Pitfalls](#common-pitfalls)

---

## Introduction

In multi-threaded C++, **atomic operations** allow threads to modify shared data without locks (mutexes), preventing data races. However, simply making a variable "atomic" is often insufficient for complex synchronization. You must also control **memory ordering**—the order in which operations become visible to other threads.

> **⚠️ Warning:** Lock-free programming is difficult. Incorrect memory ordering can lead to subtle bugs (heisenbugs) that only appear under heavy load or on specific hardware architectures (like ARM).

---

## Atomic Basics

The core template is `std::atomic<T>`. It guarantees that individual loads and stores are atomic (indivisible).

### Key Operations

| Operation | Method | Description |
| :--- | :--- | :--- |
| **Store** | `var.store(val, order)` | Writes `val` to the atomic variable. |
| **Load** | `var.load(order)` | Reads the current value. |
| **RMW** | `var.fetch_add(val, order)` | Read-Modify-Write (increment, decrement, bitwise). |
| **CAS** | `var.compare_exchange_weak(...)` | Compare-and-Swap (the primitive for lock-free logic). |

---

## Memory Ordering Models

The `std::memory_order` enum specifies how non-atomic memory operations are ordered around atomic operations.

### 1. Sequential Consistency (`seq_cst`)
**The Default / The Safest.**
* **Guarantee:** A global total order exists for all `seq_cst` operations. All threads see the exact same order of modifications.
* **Cost:** Highest (requires heavy CPU memory fences).
* **Use Case:** When you need to reason about global state or simplify logic.

### 2. Acquire-Release
**The Standard Synchronization Pattern.**
* **Release (`memory_order_release`):** Used on **Stores**. Ensures no reads/writes *before* the store are reordered to *after* the store.
* **Acquire (`memory_order_acquire`):** Used on **Loads**. Ensures no reads/writes *after* the load are reordered to *before* the load.
* **Effect:** When Thread A releases and Thread B acquires the same variable, Thread B is guaranteed to see everything Thread A wrote before the release.

### 3. Relaxed (`relaxed`)
**The Fastest / The Riskiest.**
* **Guarantee:** Only guarantees atomicity of the specific variable. **No** ordering constraints on other memory.
* **Use Case:** Incrementing statistics counters, reference counting (decrementing).

---

## Ordering Comparison

| Mode | Enum `std::` | Sync Cost | Main Guarantee |
| :--- | :--- | :--- | :--- |
| **Relaxed** | `memory_order_relaxed` | 🟢 Low | Atomicity only. No synchronization with other data. |
| **Acquire** | `memory_order_acquire` | 🟡 Medium | **Read Barrier.** Subsequent ops stay after load. |
| **Release** | `memory_order_release` | 🟡 Medium | **Write Barrier.** Preceding ops stay before store. |
| **Acq/Rel** | `memory_order_acq_rel` | 🟡 Medium | Both barriers. Used for RMW (Read-Modify-Write). |
| **Seq Cst** | `memory_order_seq_cst` | 🔴 High | **Total Global Order.** Strongest consistency. |

---

## Code Examples

### The "Handshake" (Acquire/Release)

This is the most common use case: passing data from a producer to a consumer.

```cpp
#include <atomic>
#include <string>
#include <cassert>

std::string data;
std::atomic<bool> ready{false};

void producer() {
    data = "Payload"; // 1. Non-atomic write
    
    // 2. Release Store
    // Ensures line 1 is visible before line 2 finishes
    ready.store(true, std::memory_order_release); 
}

void consumer() {
    // 3. Acquire Load
    // Loops until true. Syncs with the store above.
    while (!ready.load(std::memory_order_acquire)); 
    
    // 4. Safe Read
    // Guaranteed to see "Payload"
    assert(data == "Payload"); 
}
```

### The Counter (Relaxed)
When the order of other data doesn't matter, only the final value of the atomic itself.

```cpp
std::atomic<int> counter{0};

void increment_stats() {
    // Very fast, low overhead
    counter.fetch_add(1, std::memory_order_relaxed);
}
```

## Common Pitfalls
- Thinking volatile is atomic: In C++, volatile does not imply atomicity or thread safety. It is for hardware I/O. Always use std::atomic.
- Assuming x86 behavior is universal: Intel x86 CPUs represent a "Strong Memory Model" (loads/stores are rarely reordered). ARM (mobile/Apple Silicon) is a "Weak Memory Model." Code that works on x86 by accident might crash on ARM if memory ordering is missing.
- False Sharing: Placing two atomic variables on the same cache line (usually 64 bytes) causes cores to fight for the cache line, killing performance. Use alignas(64) to separate heavy-contention atomics.

## Resources
- [CppReference: Memory Order](https://en.cppreference.com/w/cpp/atomic/memory_order.html)