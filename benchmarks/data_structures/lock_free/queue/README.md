# Lock-free queue benchmarks

Key results (from provided runs):
- Single-thread push/pop: LockFree ≈ 6.73 ms, LockBased ≈ 4.43 ms (~1.5× slower).
- MPMC 4 producers/consumers: LockFree ≈ 4.03 ms, LockBased ≈ 2.43 ms (~1.6× slower).
- MPMC 8 producers/consumers: LockFree ≈ 58.9 ms, LockBased ≈ 13.7 ms (~4.3× slower).

Bottom line
- The lock-based queue outperforms the lock-free implementation in all measured scenarios; the gap grows with concurrency.

Likely causes:
- Per-operation dynamic allocations or allocator contention.
- High CAS retry rates / contention in the lock-free algorithm.
- Costly memory-reclamation (shared_ptr/reference counting or synchronized reclamation).
- Cache-line false sharing (head/tail not padded).
