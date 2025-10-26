# Bench notes — lock-free vs lock-based stack (observed results)

This document explains why the benchmark numbers were observed. It focuses on the implementation in this folder (a Treiber-style lock-free stack vs a mutex-protected `std::stack`-based implementation).

Environment (reported by the benchmark host)

- CPU: 16 logical cores (reported as "16 X 24 MHz CPU s" — likely 16 cores at ~2.4GHz)
- CPU caches: L1 Data 64 KiB, L1 I-Cache 128 KiB, L2 Unified 4096 KiB (per core)
- System load: Load Average ~2.09, 3.33, 3.69 (non-idle workload on machine)

Reference results (as reported)
```
-----------------------------------------------------------------------------------------------------------
Benchmark                                                                 Time             CPU   Iterations
-----------------------------------------------------------------------------------------------------------
BM_SingleThread_PushPop<LockFreeStack<int>>/100000                  1603687 ns      1603086 ns          429
BM_SingleThread_PushPop<LockFreeStack<int>>/500000                  8369407 ns      8361701 ns           87
BM_SingleThread_PushPop<LockBasedStack<int>>/100000                  785572 ns       785410 ns          893
BM_SingleThread_PushPop<LockBasedStack<int>>/500000                 3920864 ns      3919803 ns          178
BM_MPMC_Workload<LockFreeStack<int>>/4/2500/4/iterations:10         1568817 ns       191900 ns           10
BM_MPMC_Workload<LockBasedStack<int>>/4/2500/4/iterations:10         470567 ns        84500 ns           10
BM_MPMC_Workload<LockFreeStack<int>>/16/100000/16/iterations:10   886695787 ns     10474600 ns           10
BM_MPMC_Workload<LockBasedStack<int>>/16/100000/16/iterations:10   46040504 ns       306300 ns           10
-----------------------------------------------------------------------------------------------------------
```
High-level summary

- The lock-based `LockBasedStack<int>` is consistently faster than the lock-free `LockFreeStack<int>` in these benchmarks (single-threaded and multi-threaded). The gap is modest for small workloads but enormous for the largest MPMC scenario (16 producers/consumers, 100k each).
- This is not surprising given the current lock-free implementation: it contains heavy non-lock-free components (notably a global retired list guarded by a `std::mutex` and per-node `new`/`delete`), which introduce serialization and allocator contention that eclipse the benefits of lock-free CAS operations.

## Root cause analysis 

1) Serialized deferred-reclamation (global `retired_` + `std::mutex`)
- `LockFreeStack::pop()` in the current implementation does a `retired_.push_back(old_head)` under a `std::lock_guard<std::mutex>`. That single mutex is acquired by every thread that pops a node.
- In heavy MPMC workloads, that mutex becomes a hot serialization point. Although the stack `head_` updates are lock-free, pushing popped nodes to the retired list becomes a de facto global lock.
- Result: the lock-free stack actually contains a hot mutex in its hot path, removing any advantage over the lock-based implementation and adding extra overhead.

2) Per-operation allocation / deallocation
- Each `push()` allocates a new `Node` (`new Node(...)`) and popped nodes are eventually `delete`d in `clear()` or by a single-threaded cleanup. The benchmark runs create and retire many nodes, causing heavy allocator traffic and likely allocator-internal locks or contention.
- `std::stack` (used by the lock-based implementation) often has more favorable allocation patterns (e.g., `std::deque` amortized behavior) or at least fewer cross-thread new/free interactions in the specific measured scenarios.

3) CAS contention and cache-line ping-pong
- The Treiber algorithm uses CAS on a single shared pointer `head_`. With many concurrent producers/consumers, that one cache line is constantly updated and bounced between cores, creating memory traffic and CAS retries.
- CAS retries introduce wasted CPU cycles. If the implementation additionally uses locks elsewhere (retired mutex), threads may be blocked or spun, worsening overall latency.

4) OS scheduling and blocking behaviour (wall vs CPU time)
- For the very large lock-free MPMC (16x100k) you see wall time ≫ CPU time (886s wall vs 10.47s CPU aggregate). This suggests many threads were blocked or sleeping (waiting on OS synchronization / I/O or waiting on mutexes inside allocator), not using CPU while the wall clock advanced.
- This is consistent with the retired list mutex or global allocator locks causing threads to block or be descheduled, producing long wall times.

5) Memory pressure and allocator contention
- Heavy allocation and retention of retired nodes increases memory pressure. The allocator (malloc/new) may use internal mutexes, arenas, or employ expensive cross-thread bookkeeping, which serializes parts of the workload and amplifies delays in the lock-free code.

6) False expectations: "lock-free" != always faster
- Lock-free data structures remove some forms of blocking, but they still have costs: atomic instructions, retries, and (most importantly) correct memory reclamation.
- If the lock-free implementation uses locks for reclamation or does per-op allocations, these extra costs easily swamp any CAS advantage. A well-crafted lock-free design needs a low-overhead reclamation/pooling strategy to win.

Concrete evidence mapping to your numbers

- Single-threaded push/pop: lock-free is ~2x slower. In a single-threaded run the lock-free code still performs atomic ops and per-node allocations; `std::stack`'s non-atomic operations and possibly fewer allocations make it faster.

- Small MPMC (4×2.5k): lock-free slower by ~3×. This workload is too small to amortize lock-free overheads and retired-list/allocator overhead still shows.

- Large MPMC (16×100k): lock-free dramatically worse (≈20× slower or more). This is where the retired-list mutex and allocator contention explode: many pops happen concurrently, all trying to push into a single retired vector protected by a mutex; allocator locks and cache thrashing compound the penalty. The low CPU-time relative to wall time indicates many threads were stalled or blocked (e.g., waiting on malloc or the retired mutex), so overall wall time is huge even if the CPU-time is moderate.


## Fixes and recommendations (how to make lock-free competitive)

A) Avoid the global `retired_` mutex in the hot path
- Change `retired_` to a `thread_local` retired vector. Each thread will push its retired nodes locally (no locking) and the global reclamation can happen at well-defined synchronization points (benchmark teardown or occasional harvesting).
- This removes the hot global mutex and eliminates one major serialization.
- Caveat: still not a correct, general-purpose reclamation scheme — but for benchmarking and many use cases it is a practical and fast improvement.

B) Use a thread-local object pool (preallocate nodes)
- Remove `new`/`delete` from the hot path by obtaining nodes from a per-thread pool. When popping, returned nodes can be pushed back to the pool (or retired and recycled when safe).
- This removes allocator contention and memory fragmentation effects.

C) Implement safe reclamation
- Integrate hazard pointers or epoch-based reclamation (or adopt a library implementation). This provides correctness and safety while enabling reclamation of nodes without a global lock.
- There is a runtime overhead for such schemes, but they are much better than an ad-hoc global mutex for concurrency.

D) Use better backoff and reduce CAS retries
- Introduce exponential or randomized backoff in tight CAS loops to reduce contention during bursts.

E) Consider alternative data structures
- For heavy multi-producer/multi-consumer workloads consider designs that partition work (per-producer queues + combining) or use lock-striping to reduce contention.
