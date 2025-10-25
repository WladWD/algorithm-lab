# Lock-free Stack

A lock-free stack is a concurrent stack data structure that provides thread-safe push/pop operations without using mutual-exclusion locks (mutexes). Instead, it relies on atomic primitives (typically compare-and-swap / CAS) and careful memory management so that multiple threads can make progress concurrently.

## Key concepts

- Lock-free: At least one thread makes progress in a finite number of steps. No global locks block all threads.
- Wait-free: Every thread makes progress in a bounded number of steps (stronger than lock-free).
- Atomic primitives: Implementations typically use CAS (compare-and-swap) on a pointer-sized word to update the stack head.

## Basic idea (single-word CAS stack)

A classical lock-free stack stores a pointer to the head node and uses CAS to atomically swap the head when pushing or popping:

- push(node):
  1. Read current head
  2. Set node->next = head
  3. CAS(&head, head, node) — retry on failure

- pop():
  1. Read head
  2. If head == nullptr -> empty
  3. next = head->next
  4. CAS(&head, head, next) — retry on failure
  5. return popped node

Because CAS only succeeds if the head didn't change since it was read, concurrent pushes/pops are serialized only at the atomic update; threads that lose the race retry and try again. This avoids blocking entire threads on a mutex.

## Difference vs lock-based stack

Lock-based (mutex) stack:
- Uses a mutex to guard the head pointer and node modifications.
- If one thread holds the mutex, other threads block (sleep/wait) until it is released.
- Simpler to implement and reason about; default choice for many applications.

Lock-free stack:
- Uses atomic operations, no mutex. Threads may retry on CAS failure but are never blocked by another thread's lock hold.
- Better latency under certain contention patterns because threads don't get descheduled by the OS while waiting for a lock.
- Can give higher throughput and lower tail latencies in low-to-moderate contention workloads.

Trade-offs:
- Lock-free code is more complex and error-prone (ABA, memory reclamation, subtle invariants).
- Under very high contention or with heavy retries, lock-based implementations with backoff or scoped locks can be simpler and as efficient.

## Why choose lock-free?

- Reduced blocking and priority inversion: since no thread holds a global lock, other threads aren't blocked by a long-running critical section.
- Lower tail latency: avoids context switches caused by threads being blocked on mutexes.
- Better scalability for short critical sections and high concurrency.

## Common pitfalls

1. ABA problem
- Scenario: Thread A reads head==X, is preempted. Thread B pops X, then pops other nodes and pushes a new node that happens to reuse the same address X. When thread A resumes, head==X still, so CAS succeeds but the stack state is different.
- Mitigations:
  - Tag/counter the pointer (pointer + version) so CAS checks both pointer and counter (so address reuse is distinguished).
  - Use double-width CAS (DCAS) or 128-bit CAS when available (pointer + tag in one atomic word).
  - Use safe memory reclamation to avoid reusing freed addresses too quickly.

2. Memory reclamation
- You cannot free a popped node immediately because another thread might still hold a reference to it (because it read head earlier). Reclaiming memory incorrectly leads to use-after-free bugs.
- Common strategies:
  - Hazard pointers: Threads publish which nodes they might access; freed nodes are reclaimed only when no hazard pointer references them.
  - Epoch-based (RCU-like) reclamation: Reclaim nodes only after all threads have advanced past the epoch in which they might reference the node.
  - Reference counting (shared_ptr): Simpler but with overhead and potential ABA-like issues with cyclic structures.
  - Deferred reclamation / garbage collection.

3. Starvation and fairness
- Lock-free guarantees system-wide progress but not per-thread fairness. A thread may be repeatedly unlucky and suffer many retries.

4. Complexity and testing
- Lock-free algorithms are subtle; unit tests, stress tests under thread scheduling pressure, and sanitizer runs are essential.

## Progress guarantees

- Lock-free: ensures system-wide progress (some thread completes) but individual threads may starve.
- Wait-free: stronger guarantee (all threads complete in bounded steps) but usually more complex and less practical for many data structures.

This implementation is expected to be lock-free (not wait-free).

## Testing and benchmarking

- Unit tests: correctness under single-threaded and multi-threaded scenarios (interleavings). Use small deterministic sequences to validate core invariants.
- Stress tests: run high-concurrency stress tests with many threads and long run times. Randomized operations (push/pop) and thread preemption help reveal bugs.
- Sanitizers: ASan/UBSan are useful for detecting memory errors; ThreadSanitizer (TSan) is helpful for data race detection but can produce false positives for lock-free algorithms that rely on atomic operations — still useful.
- Benchmarking: measure throughput (ops/sec) and latency (tail percentiles). Compare with a mutex-protected stack baseline. Watch for allocator overhead; consider using an object pool to isolate container behavior from allocation cost.

## Profiling and performance tips

- Minimize dynamic allocations in hot paths; pre-allocate nodes (free list / object pool) when possible.
- Use small node sizes and avoid extra indirection to help cache locality.
- Add exponential backoff on CAS retry loops under high contention to reduce bus traffic.
- Measure both throughput and latency (including p99/p999) since lock-free structures often improve tail latency even when throughput gain is modest.

## When not to use lock-free

- If correctness and simplicity are primary and contention is low, prefer a mutex-based implementation.
- If memory reclamation strategies are not available (or too costly), lock-free may not be safe.
- When you need strong per-thread fairness or bounded per-thread latency (wait-free semantics), and a wait-free algorithm is not available.

## References and further reading

- M. Herlihy, N. Shavit — The Art of Multiprocessor Programming
- D. Lea — Concurrent Programming in Java (for background on locks and concurrent collections)
- Hazard pointers: Maged M. Michael — Hazard Pointers: Safe Memory Reclamation for Lock-Free Objects
- Epoch reclamation / RCU literature
- Papers and blog posts about the ABA problem and lock-free data structures
