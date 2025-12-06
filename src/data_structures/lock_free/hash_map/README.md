# Lock-Free Hash Map

## Overview

A lock-free hash map provides concurrent insert/erase/find operations without global locks. It aims to offer low-latency and good scalability under concurrent workloads by using atomic primitives and careful memory-reclamation techniques instead of mutexes.

Lock-free hash maps are useful in low-latency servers, network stacks, and real-time systems where blocking is undesirable.

---

## Design variants

There are two common approaches to building a concurrent hash map:

1. Open addressing (probing)
   - Entries are stored directly in an array (table) using a probing scheme (linear, quadratic, Robin Hood).
   - Lock-free open addressing is challenging due to in-place moves and the need to update multiple slots atomically.
   - Pros: compact memory layout, good cache locality.
   - Cons: complex to make fully lock-free and to support deletes safely.

2. Chained hash map (buckets with linked lists or arrays)
   - Each bucket holds a list (or small array) of entries. Lock-free chaining typically uses atomic pointers and safe reclamation.
   - Easier to implement in a lock-free fashion using per-bucket CAS and safe memory reclamation.
   - Pros: easier to support deletes and resizing with well-known techniques.
   - Cons: pointer chasing (poorer cache locality).

For a general-purpose lock-free map, the chained approach with careful reclamation is often the practical choice.

---

## Lock-free techniques (patterns)

Key building blocks and patterns:

- Atomic pointer updates (CAS): used to insert/remove nodes in bucket lists without locks.
- Versioned pointers / tagged pointers: combine a pointer and a version counter to mitigate ABA problems.
- Per-bucket operations: keep operations mostly local to a bucket to reduce contention.
- Resize / rehash strategy: incremental rehashing or versioned tables to allow concurrent readers/writers during resize.
- Avoid global pauses: use non-blocking algorithms for rehashing or perform rehashing in small steps.

A typical insert proceeds by allocating a new node, setting the node's next pointer to the current bucket head, and then CAS-ing the bucket head from old to new. Delete uses CAS to swing next pointers and safe reclamation to avoid freeing memory still referenced by other threads.

---

## Memory reclamation

Correct memory reclamation is critical. Several common approaches:

- Hazard pointers
  - Threads publish pointers they may dereference; deleted nodes are reclaimed only after no hazard pointer references them.
  - Deterministic, safe, but requires bookkeeping and some per-thread cost.

- Epoch-based reclamation (quiescent-state based)
  - Threads advance epochs and retired nodes are reclaimed after all threads pass the epoch where they could reference the node.
  - Low overhead and typical good throughput; requires all participants to cooperate.

- Reference counting (shared_ptr)
  - Simple but incurs atomic reference-count costs on each access; may be too heavy for hot paths.

Choose a reclamation scheme before implementing the data structure; many lock-free maps assume hazard pointers or epoch-based reclamation.

---

## API and usage model

Minimal API:

```cpp
template <typename K, typename V>
class LockFreeHashMap {
public:
    bool insert(const K& key, const V& value); // false if key exists
    bool erase(const K& key); // false if not found
    std::optional<V> find(const K& key);
    // optional: upsert, for_each, size (approx), clear
};
```

Design notes:
- `find` should be wait-free or lock-free and must not return references to internal nodes unless the caller acquires hazard protection.
- `insert`/`erase` may be lock-free but require memory-reclamation coordination.

---

## Example C++ sketch (chained map)

This is a concise illustrative sketch (not production-ready). It assumes an external reclamation scheme (e.g., hazard pointers) is used and omits error handling and resizing.

```cpp
struct Node { K key; V value; std::atomic<Node*> next; };

std::atomic<Node*> buckets[N];

bool insert(const K& k, const V& v) {
    size_t b = hash(k) & (N-1);
    Node* newNode = new Node{k, v, nullptr};
    while (true) {
        Node* head = buckets[b].load(std::memory_order_acquire);
        newNode->next.store(head, std::memory_order_relaxed);
        if (buckets[b].compare_exchange_weak(head, newNode, std::memory_order_release, std::memory_order_acquire))
            return true;
    }
}

std::optional<V> find(const K& k) {
    size_t b = hash(k) & (N-1);
    Node* cur = buckets[b].load(std::memory_order_acquire);
    while (cur) {
        if (cur->key == k) return cur->value;
        cur = cur->next.load(std::memory_order_acquire);
    }
    return std::nullopt;
}
```

To delete a node you must carefully CAS the previous node's `next` pointer and retire the removed node using the reclamation mechanism.

---

## Complexity and performance

- Average lookup/insert/delete: O(1) expected assuming a good hash and moderate load factor.
- Memory: O(n) nodes + bucket table.
- Performance depends heavily on contention and the reclamation strategy. Avoid heavy per-operation synchronization and prefer per-bucket locality.

---

## References

- M. Michael: Hazard Pointers and lock-free algorithms


