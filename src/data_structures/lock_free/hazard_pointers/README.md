# Safe Memory Reclamation (SMR)
### Lock-Free Memory Management: Hazard Pointers & Epoch Reclamation

This repository implements the two industry-standard approaches to solve this:
1.  **Hazard Pointers (HP):** Deterministic, strictly bound memory usage, wait-free readers.
2.  **Epoch Reclamation (ER):** High throughput, batch processing, extremely low reader overhead.

---

## The Problem: Concurrent Deletion

In a concurrent environment, one thread cannot simply `delete` a node removed from a data structure because other threads may still be reading it.

### The Race Condition
1.  **Thread A** reads a pointer to `Node X`.
2.  **Thread B** removes `Node X` from the structure and immediately `deletes` it.
3.  **Thread A** attempts to access `Node X` → **Undefined Behavior (Segfault/Corruption).**

We require a **Safe Memory Reclamation (SMR)** scheme to guarantee that `Node X` is only destroyed once no threads hold references to it.

---

## 🛠️ Strategy 1: Hazard Pointers

Hazard Pointers (Michael & Scott, 2004) allow threads to "protect" specific pointers they are accessing. A thread publishes a pointer it intends to read into a global array (the Hazard Array).

### Algorithm
1.  **Reader:** Before accessing a node, the thread marks it in its thread-local Hazard Pointer slot ("I am reading this").
2.  **Writer (Retire):** When removing a node, the thread adds it to a thread-local "retired list."
3.  **Reclamation (Scan):** When the retired list reaches a threshold $R$, the thread scans the global Hazard Array.
    * If a retired node is found in the Hazard Array, it is still in use (Skipped).
    * If it is not found, it is safe to `delete`.

### Pros & Cons
* ✅ **Lock-Free / Wait-Free:** Readers never block.
* ✅ **Bound Memory:** Limits the number of unreclaimed objects.
* ⚠️ **Overhead:** Requires a memory barrier (Store-Load fence) on every read to ensure the "announcement" is visible.

### Code Example
```cpp
// Reader Thread
HazardPointer* hp = get_hazard_pointer();
Node* ptr;
do {
    ptr = atomic_head.load();
    hp->store(ptr); // Announce intention to read
} while (ptr != atomic_head.load()); // Validation check

// Safe to read ptr...
read_data(ptr);

hp->clear(); // Release protection
```

## 🛠️ Strategy 2: Epoch Reclamation

Epoch-based reclamation (EBR) is a high-throughput, timestamp-based approach to safe memory reclamation. Unlike Hazard Pointers, which track individual pointers, EBR tracks the progress of threads through execution "epochs" to determine when memory is safe to free in batches.

### How It Works

The system maintains a **Global Epoch** counter and tracks **Local Epochs** for each thread.

1.  **Global Epoch ($E_G$):** A central atomic counter representing the current logical time window.
2.  **Local Epoch ($E_L$):** Each thread publishes its current epoch state. To access protected data, a thread must "enter" the epoch by setting its $E_L$ to match $E_G$.
3.  **Retirement:** When a node is removed/unlinked during epoch $E$, it is placed in a "limbo list" (retire list) associated with that specific epoch. It is **not** immediately deleted.
4.  **Reclamation Rules:** A node removed in epoch $E$ can be safely `deleted` only when **all active threads** have advanced their local epochs to at least $E + 1$.
    * *Logic:* If all threads are in epoch $E+1$ (or higher), no thread can possibly be holding a reference to an object removed in epoch $E$, because they would have had to cross a memory barrier to update their local epoch.

### ✅ Pros
* **Performance:** Extremely low overhead for readers. It usually involves just reading/writing a thread-local flag or integer, which caches well.
* **Batching:** Memory is reclaimed in large chunks, reducing allocator pressure.
* **Scalability:** scales extremely well with the number of readers.

### ⚠️ Cons
* **Blocking Potential:** If a single thread halts, crashes, or sleeps while in an "active" epoch state, the global epoch cannot advance. This prevents memory reclamation for *all* threads (unbounded memory growth).
* **Non-Deterministic:** You cannot predict exactly when memory will be freed.

### Code Example

```cpp
// 1. Thread enters the critical section (updates local epoch)
epoch_guard guard; 

// 2. Thread reads the atomic pointer
Node* ptr = atomic_head.load(std::memory_order_acquire);

if (ptr) {
    // It is safe to dereference 'ptr' as long as 'guard' is alive.
    // Even if another thread calls retire(ptr), the memory won't 
    // be physically deleted until we exit this scope.
    process_data(ptr->data);
}

// 3. Guard destructor exits the epoch (syncs local state)
```

## References
- M. M. Michael. "Hazard Pointers: Safe Memory Reclamation for Lock-Free Objects." IEEE Trans. Parallel Distrib. Syst. (2004).
- K. Fraser. "Practical Lock-Freedom." PhD Thesis, Cambridge University (2004).
- P. E. McKenney. "RCU: A High-Performance Scalable Lightweight Read-Side Synchronization Mechanism."