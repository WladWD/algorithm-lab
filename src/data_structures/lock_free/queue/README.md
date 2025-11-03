# Lock-free Queue (Michael–Scott)

This directory contains a header-only implementation of a lock-free queue (Michael & Scott algorithm) and supporting files. This README explains what a lock-free queue is, how the Michael–Scott queue works at a high level, its benefits compared to a traditional lock-based queue, common pitfalls (ABA, memory reclamation, fairness), complexity, and practical guidance for testing and benchmarking.

What is a lock-free queue?
- A lock-free queue is a concurrent queue data structure where multiple threads can enqueue and dequeue concurrently without using a global mutex to serialize access. Instead, it uses atomic primitives (typically compare-and-swap / CAS) to coordinate concurrent updates.
- The classic lock-free queue design is the Michael & Scott (MS) queue (1996). It provides a practical, widely used algorithm for a multi-producer / multi-consumer (MPMC) queue.

How the Michael & Scott queue works (high level)
- The MS queue uses a singly-linked list with a dummy (sentinel) node. Two atomic pointers are maintained:
  - `head` points to the node whose `next` is the first element (a sentinel for dequeues),
  - `tail` points to the last node in the list (used to append new nodes).

- Enqueue (push):
  1. Allocate a new node with the value and `next = nullptr`.
  2. Repeatedly read `tail` and `tail->next`.
  3. If `tail->next` is null, try CAS to link the new node: `CAS(&tail->next, nullptr, new)`.
  4. After linking, try to advance `tail` to the new node (helping other enqueuers).

- Dequeue (pop):
  1. Repeatedly read `head`, `tail`, and `head->next`.
  2. If `head == tail` and `head->next == nullptr`, the queue is empty.
  3. Otherwise, read the value from `head->next` and try CAS to swing `head` to `head->next`.
  4. The old head (the previous sentinel) can be retired (but must not be freed while other threads may still access it).

## Complexity
- Time: amortized O(1) per enqueue/dequeue (each operation does a bounded number of atomic steps). In practice cost depends on contention.
- Space: O(n) for n enqueued elements. Additional per-thread retired storage may be used for safe reclamation.

## Benefits vs lock-based queues
- Lower tail latency: no thread is blocked by another thread holding a mutex; threads only retry when they lose CAS races.
- Better scalability for short critical sections under contention because threads avoid syscalls/context-switches that can occur with blocked mutexes.
- On modern multicore CPUs lock-free queues can give higher throughput and smoother latency under light-to-moderate contention.

## Common issues and trade-offs
- Memory reclamation (use-after-free): after a thread removes a node, other threads might still hold references to it; freeing that node immediately can cause use-after-free. This is the most important practical issue.
- ABA problem: pointer values can change A→B→A; CAS only compares raw pointer values and can be tricked into succeeding when the logical state changed. Mitigations: pointer tagging (version counters), hazard pointers, epoch reclamation.
- Contention on head/tail cache lines: heavy concurrent enqueues/dequeues cause cache-line ping-pong and CAS retries.
- Starvation & fairness: lock-free guarantees system-wide progress (lock-free) but not per-thread fairness; unlucky threads may retry often.
- Complexity and correctness: implementing safe reclamation (hazard pointers / epochs) is non-trivial.

## Reclamation strategies (overview)
- Hazard pointers: threads publish pointers they might dereference; nodes are reclaimed only when no hazard pointer references them. Good safety and reasonable performance.
- Epoch-based reclamation: retire nodes by epoch and reclaim them only after all threads have advanced; good throughput for many workloads.
- Per-thread pools + delayed reuse: practical short-term approach for benchmarks (reduce allocator contention and ABA probability) but not a formal correctness guarantee.
- shared_ptr (reference counting): easy but costly (atomic refcounts per access).

## References
- M. Michael and M. Scott, "Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms", PODC 1996.
- M. M. Michael, "Hazard Pointers: Safe Memory Reclamation for Lock-Free Objects".
- Concurrency chapters in "The Art of Multiprocessor Programming" by Herlihy & Shavit.

