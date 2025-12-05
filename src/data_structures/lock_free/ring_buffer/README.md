# Ring Buffer (Circular Queue)


## Overview

A ring buffer stores items in an array of size N and maintains two indices:
- `head` — index of the next element to be consumed
- `tail` — index of the next slot to be produced

Indices advance modulo `N`. Producers write at `tail` and then advance it; consumers read from `head` and advance it. In lock-free implementations these indices are typically atomic counters with appropriately chosen memory orderings.

Ring buffers are widely used in streaming, networking, logging, and real-time systems where predictable latency and minimal allocation jitter are required.

---

## Variants

Choose the variant that fits your concurrency model:

- SPSC — Single-Producer / Single-Consumer
  - Simplest, fastest. Often requires only relaxed atomics and a single release-acquire pair to publish/consume.

- MPSC — Multiple-Producers / Single-Consumer
  - Producers claim slots via an atomic ticket (fetch_add) and publish the element; consumer remains single-threaded.

- SPMC — Single-Producer / Multiple-Consumers
  - Symmetric to MPSC with consumer-side coordination.

- MPMC — Multiple-Producers / Multiple-Consumers
  - The most general and complex: usually needs per-slot sequence counters and CAS to guarantee correctness.

In this repository we focus on SPSC and MPSC patterns because they are simpler to reason about and perform well for common systems workloads.

---

## Correctness invariants & capacity conventions

Let `N` be the buffer array length (power-of-two is recommended). Common conventions:

- Empty condition (one-slot reserved):

```
head == tail        // buffer is empty
```

- Full condition (one-slot reserved):

```
(tail + 1) % N == head   // buffer is full
```

The one-slot-reserved convention is simple and avoids ambiguity between full and empty states. If full-capacity usage is required, use sequence counters that encode the producer/consumer epoch per-slot.

Power-of-two `N` allows replacing `% N` with `& (N - 1)` for cheaper masking.

---

## Implementation patterns and recipes

Practical recipes and common optimizations:

- Use power-of-two capacity and index masking: `idx & (N - 1)`.
- Pad or align `head` and `tail` to separate cache lines (e.g. `alignas(64)`) to avoid false sharing between producers and consumers.
- For SPSC:
  - Producer: write payload into slot, then publish by writing `tail` with `memory_order_release`.
  - Consumer: read `tail` with `memory_order_acquire` and then load payload.
  - Many implementations can use `memory_order_relaxed` for loads/stores that are internal and do not synchronize.

- For MPSC:
  - Producers use `ticket = prod_index.fetch_add(1, relaxed)` to claim a slot index.
  - Producers write into `slots[ticket & mask]` and then publish the slot with a release-store (e.g. increment per-slot sequence or flip a flag).
  - Consumer reads per-slot sequence/flag with acquire semantics to ensure visibility.

- Sequence-based approach (Vyukov-style) for MPMC: each slot carries a sequence number that encodes its epoch; producers and consumers use that number to detect readiness and ownership without heavy CAS on every operation.

---

## Memory-ordering and false sharing

Typical memory-ordering recipe (SPSC):

- Producer: construct payload into slot (non-atomic writes) -> `std::atomic_thread_fence(std::memory_order_release)` or `tail.store(next, std::memory_order_release)`.
- Consumer: read `tail` with `std::memory_order_acquire` before loading the slot data.

For MPSC/MPSC patterns combine relaxed `fetch_add` with a release publish step to avoid excessive fencing.

Always align per-index atomics to cache-line boundaries:

```cpp
alignas(64) std::atomic<size_t> head;
alignas(64) std::atomic<size_t> tail;
```

This avoids cache-line ping-pong when head and tail are updated by different threads.

---

## API design and usage examples (C++)

Minimal API (recommended):

```cpp
template <typename T>
class RingBuffer {
public:
    explicit RingBuffer(size_t capacity);
    bool try_enqueue(const T &value); // non-blocking, returns false when full
    bool try_dequeue(T &out);          // non-blocking, returns false when empty
    size_t capacity() const noexcept;
    size_t size() const noexcept;      // approximate under concurrency
};
```

SPSC example (high-level snippet):

```cpp
SpscRing<int> q(1024);
q.try_enqueue(42);
int value;
if (q.try_dequeue(value)) {
    // consumed
}
```

MPSC sketch (producer):

```cpp
// producer thread
uint64_t ticket = prod_idx.fetch_add(1, std::memory_order_relaxed);
Slot &s = slots[ticket & mask];
// write payload into s.data
// publish: s.seq.store(ticket + 1, std::memory_order_release);
```

Consumer checks `s.seq` with acquire semantics and consumes ready slots.

---

## Complexity and performance characteristics

- Time: O(1) amortized for enqueue/dequeue (constant-time arithmetic and atomic ops).
- Space: O(N) capacity. With sequence counters per-slot add O(N) metadata.
- Contention: SPSC has minimal contention; MPSC introduces atomic `fetch_add` contention among producers; MPMC may suffer CAS retry storms under high contention.

Performance tuning checklist:
- Use `-O3 -march=native` when benchmarking.
- Tune buffer size relative to working-set and cache sizes.
- Pin threads (CPU affinity) for stable latency measurements.

---

## Pitfalls and practical tips

- Do not free or reuse nodes early if other threads might still access them; use safe reclamation strategies when nodes are dynamically allocated.
- Beware of integer overflow on indices: use sufficiently wide counters (64-bit) or mask indices carefully.
- Prefer sequence-number-based designs for MPMC variants to avoid ABA issues.
- Avoid hidden allocations under the hot path (use preallocated buffers).

---

## References

- M. Michael & M. Scott, "Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms" (1996).
- Dmitry Vyukov: MPMC queue and sequence-based ring buffer patterns (blog posts / sources).
- Anthony Williams, "C++ Concurrency in Action" — chapter on lock-free algorithms.
- Herb Sutter, concurrency talks and articles on lock-free design.

