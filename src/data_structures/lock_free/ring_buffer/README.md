# Ring Buffer (Circular Queue)

## Overview

A ring buffer stores elements in a fixed-size circular array. Producers write items at the "tail" and consumers read items from the "head". When implemented properly, ring buffers provide constant-time enqueue/dequeue and excellent cache behavior.

A ring buffer is especially attractive in producer-consumer scenarios where allocating per-element objects is expensive or where low allocation jitter is critical.

## Common variants

- SPSC: single producer, single consumer — the simplest and fastest variant; can be implemented without locks and without atomic CAS in many cases.
- MPSC: multiple producers, single consumer — typically implemented with atomic fetch_add for producers and a plain consumer index.
- SPMC: single producer, multiple consumers — symmetrical to MPSC.
- MPMC: multiple producers and multiple consumers — the most general; implementations usually require CAS/compare_exchange and careful progress guarantees.

Most low-latency systems use SPSC or MPSC variants because they are simpler and faster.

## Correctness invariants

Let capacity = N (usually power of two). Keep two indices:
- head — next element to read (consumer index)
- tail — next slot to write (producer index)

Empty: head == tail
Full: (tail + 1) % N == head  (when using one slot to distinguish full/empty)

Alternative: track size or use sequence counters (Lamport/Cohen patterns) to allow full-capacity usage; but the single-empty-slot approach is simplest.

## Efficient implementations

Guidelines:
- Use power-of-two capacity and index masking: index & (N - 1) to avoid modulo.
- Pad head/tail to separate cache lines to avoid false sharing between producer and consumer.
- For SPSC you can use relaxed memory ordering for many operations, using a release-acquire pair for publish/consume.
- For MPSC producers typically use atomic fetch_add (relaxed semantics) to claim a slot and then publish the element with a release store; the consumer uses acquire loads.

### SPSC (fastest)
- No CAS required.
- Producer updates tail (store) after writing the value with release semantics.
- Consumer reads head and the slot with acquire semantics and then advances head.
- Example: a standard circular array with atomic<uint64_t> head, tail; but only one writer/reader touches each index.

### MPSC (practical)
- Producers use fetch_add on a single atomic counter to obtain a ticket/slot index.
- Producers write data into the claimed slot and then perform a release-store to a per-slot sequence or flag to mark it ready.
- Consumer scans slots (or uses a sequence counter) and performs acquire reads when consuming.
- This pattern avoids heavy CAS on every enqueue and works well when writes are frequent and contention is moderate.

### MPMC (complex)
- Requires per-slot CAS for correct handoff or more complex fencing with sequence numbers.
- Consider using established libraries (folly::ProducerConsumerQueue, Boost.Lockfree) unless you need a custom implementation.

## Memory-ordering and false-sharing

- Use `alignas(64)` or explicit padding for indices and per-slot metadata to avoid false sharing.
- For SPSC:
  - Producer: write element -> std::atomic_thread_fence(std::memory_order_release) or release-store to tail.
  - Consumer: read tail with acquire semantics to observe published elements.
- For MPSC:
  - Producers: use atomic fetch_add (acquire-release or relaxed + release publish step) to claim slots.
  - Consumers: use acquire loads to read published flags.

Prefer simple and well-understood orderings; avoid mixing many fences unless necessary.

## References

- L. M. Lamport: "Concurrent Programming" patterns and the circular buffer concept.
- Dmitry Vyukov: MPMC queue algorithms and sequence-based ring buffer techniques.
- Anthony Williams, "C++ Concurrency in Action" on lock-free patterns.



