# Hash Table (Separate Chaining) — Proof of Correctness

## Model

The table is an array of `capacity` buckets. Each bucket is an ordered list of `(key, value)` pairs. A key `k` resides in bucket `h(k) % capacity`. All keys in the same bucket are stored in insertion order.

---

## Correctness of `find`

**Claim:** `find(k)` returns a pointer to the value associated with `k`, or `nullptr` if absent.

**Proof:** `bucket_for(k)` maps `k` to `buckets_[h(k) % cap]`. The linear scan of that bucket checks every pair with `equal_`. Since every inserted pair (key `k`, value `v`) is placed in exactly the bucket `h(k) % cap`, and `find` scans the same bucket, the pair is found iff it was inserted and not yet erased. ✓

---

## Correctness of `erase`

`erase` locates the entry in `bucket_for(k)` and calls `bucket.erase(it)`, which removes it from the vector. No tombstones are needed because scanning an empty position is not possible — the vector contains only live entries. ✓

---

## Correctness of `insert` (update semantics)

When `insert(k, v)` is called and `k` is already present, the scan of `bucket_for(k)` finds the pair and overwrites the value. No duplicate is created, and `size_` is not incremented. ✓

---

## Load-factor invariant and rehash

**Invariant:** After every public operation, `size_ / capacity ≤ kMaxLoadFactor` (1.0).

- `insert` checks `size_ + 1 > capacity * 1.0` before insertion. If true, the table rehashes to `2 × capacity` before the new entry is added.
- After rehash the load drops to approximately `size_ / (2 × old_cap) ≤ 0.5`.
- `erase` decrements `size_`, reducing load immediately. ✓

**No tombstone accumulation:** unlike open addressing, erased entries are removed from the bucket and do not inflate `size_` or any occupancy counter. Rehash is triggered purely by `size_` growing. ✓

---

## Expected O(1) operation time

With a uniform hash and load factor α ≤ 1:

- **Successful lookup:** Expected comparisons = 1 + α/2 ≤ 1.5.
- **Unsuccessful lookup:** Expected comparisons = α ≤ 1.
- Both are O(1). ✓

---

## Amortised O(1) insert

Each `insert` may trigger at most one rehash. A rehash doubles capacity and costs O(size). By the standard doubling argument the total rehash work over n inserts is O(n), giving O(1) amortised per insert. ✓

---

## Space: O(capacity + size)

- Bucket-array overhead: O(capacity) — one `vector` header per bucket.
- Live entries: O(size) — one `pair` per entry.
- Total: O(capacity + size). Since capacity ≥ size / α ≥ size, this is O(size) after the last rehash. ✓

---

## References

- T. H. Cormen et al., *Introduction to Algorithms* 4th ed., §11.2 — chaining analysis.
- D. Knuth, *The Art of Computer Programming Vol. 3*, §6.4.
