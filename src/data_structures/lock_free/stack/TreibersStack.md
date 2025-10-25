# Treiber's Stack — overview

This document describes Treiber's stack (a simple lock-free stack), what problem it solves, and common implementation issues to watch out for.

## What is Treiber's stack?
Treiber's stack is a classic lock-free implementation of a stack (LIFO) data structure for concurrent environments. It was introduced by Robert Treiber in the 1980s and uses an atomic compare-and-swap (CAS) on a single pointer (the head) to coordinate concurrent push and pop operations without using locks.

At a high level:
- The stack stores nodes linked via a `next` pointer.
- `push(node)` reads the current head, sets `node->next = head`, and does `CAS(&head, head, node)` to install the new head.
- `pop()` reads head, reads `head->next`, and does `CAS(&head, head, next)` to remove the head when CAS succeeds.

Because CAS retries on concurrent modifications, no thread ever holds a global mutex — threads that lose the race retry their operation.

## What problem does it solve?
- Provides a thread-safe LIFO stack without mutual-exclusion locks.
- Eliminates blocking and kernel-level context switches that can occur when threads wait on a mutex under contention.
- Reduces tail latency for short critical sections because threads spin/retry instead of being descheduled.
- Often improves throughput and latency in high-concurrency workloads with short operations.

Use-cases:
- Work-stealing or task pools where push/pop are frequent and short.
- Low-latency producer-consumer paths where reducing blocking is important.

## Complexity
- Amortized (per operation): O(1) expected for push/pop (subject to retries under contention).
- Space: O(n) nodes.

## Common implementation issues and pitfalls
Treiber's stack is conceptually simple, but correct and robust implementations must address several concurrency hazards:

1. ABA problem
- Description: A thread reads `head == A`, is preempted. Other threads pop A, pop other nodes, and push a new node that reuses the same memory address A. When the first thread resumes, `head == A` still holds (same pointer value) and CAS succeeds, but the stack state has changed unexpectedly.
- Consequence: Logical corruption or lost nodes.
- Mitigations:
    - Use a tagged pointer (pointer + small counter) so the CAS checks both pointer and version.
    - Use wider CAS (e.g. 128-bit) if available to atomically update pointer+tag.
    - Prevent address reuse quickly by using safe reclamation (hazard pointers / epoch reclamation).

2. Memory reclamation (safe deletion)
- Problem: After a successful pop, you cannot free the popped node immediately because other threads may still hold references to it (they may have read the pointer earlier). Freeing it too early leads to use-after-free.
- Strategies:
    - Hazard pointers: each thread publishes pointers it might access; nodes are freed only after no hazard pointer references them.
    - Epoch-based reclamation: nodes are reclaimed only after all threads have moved past an epoch in which the node could be referenced.
    - Reference counting (shared_ptr): simpler but can be slower and may not handle cycles.
    - Deferred reclamation: accumulate retired nodes and reclaim them when safe (requires a safe global point or quiescence).

3. Starvation and fairness
- Lock-free guarantees global progress (some thread makes progress) but not per-thread fairness. A thread may repeatedly lose CAS races and starve.
- Mitigation: use backoff strategies, cooperative yielding, or combine with mutex fallback if fairness is required.

4. Contention and performance
- Under very high contention, repeated CAS failures create memory bus traffic and cache-line ping-pong. Backoff (exponential or randomized) reduces contention.
- Consider batching or combining techniques for heavy workloads.

5. Pointer alignment / false sharing
- Ensure node layout and head pointer sit on separate cache lines as needed to reduce false sharing with unrelated state.

6. Exception safety and ownership
- If node allocation or construction throws, ensure invariants are preserved. Prefer constructing node before linking it into the stack.

## Practical tips for implementations
- Keep the push/pop fast and allocation-free in the hot path: pre-allocate nodes or use an object pool when possible.
- Add exponential backoff on CAS failures under contention.
- Test under stress with thread preemption, and run sanitizers (ASan/UBSan) and ThreadSanitizer (TSan) for data races — TSan may produce false positives for benign atomic usage but is still useful.
- Provide an API that separates allocation from the critical push/pop path when low-latency is important (e.g., caller allocates node then calls push(node)).

## Example (pseudocode)
```
// push
node->next = head
while (!CAS(&head, node->next, node)) { /* retry */ }

// pop
old = head
while (old != NULL) {
  next = old->next
  if (CAS(&head, old, next)) {
    // success, reclaim old safely
    return old->value
  }
}
return EMPTY
```

## Testing and verification
- Unit tests for single-threaded correctness.
- Deterministic concurrency tests (small interleavings) where feasible.
- Stress tests with many threads and long runs.
- Instrumentation counters for counts of CAS retries, pushes, pops, retire counts.

## References
- R. Treiber, "Systems Programming: Coping with Parallelism", 1986.
- M. Herlihy & N. Shavit, "The Art of Multiprocessor Programming".
- Maged M. Michael, "Hazard Pointers: Safe Memory Reclamation for Lock-Free Objects".



