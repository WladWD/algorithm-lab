# Hash Table (Open Addressing) — Proof of Correctness

## Model

The table is a flat array of `capacity` slots. Each slot has one of three states:

- **Empty** — never used.
- **Occupied** — holds a live key-value pair.
- **Deleted** (tombstone) — previously occupied, now erased.

The hash function maps each key to a start index `h(k) % capacity`. Linear probing resolves collisions by checking slots `(h(k) + 0), (h(k) + 1), …` modulo capacity.

---

## Correctness of `find_slot`

**Claim:** `find_slot(k)` returns the index of the unique Occupied slot holding `k`, or `capacity` if `k` is absent.

**Proof:** Starting at `h(k) % cap`, the probe visits slots in order. The probe stops at the first Empty slot. Because `insert` (see below) never leaves an Empty slot between `h(k)` and the insertion point, all Occupied slots for keys with hash `h(k)` appear before the first Empty slot in the probe sequence. Tombstone slots are skipped (they were Occupied when `k` could have been inserted, so `k` may lie beyond them). If `k` exists it is reached before the first Empty; if it does not exist the first Empty terminates the search. ✓

---

## Correctness of `insert_slot`

**Claim:** `insert_slot(k)` returns the index at which `k` should be stored: the existing Occupied slot if `k` is present, or the first Deleted slot encountered (tombstone reuse), otherwise the first Empty slot.

**Proof:** The probe sequence is the same as `find_slot`. On the way to the first Empty (or end of probe), the first Deleted slot is recorded. If `k` is found (Occupied, equal key), return immediately. Otherwise return the first Deleted if any (reusing reclaimed space avoids wasting slots), or the first Empty. Both choices keep the invariant that no Empty slot separates `h(k)` from the stored position of `k`. ✓

---

## Correctness of `erase`

Erasing marks the slot Deleted rather than Empty. This preserves the invariant used by `find_slot`: subsequent probes for any key that was inserted after `k` (and whose probe sequence passed through this slot) will continue past the tombstone rather than stopping prematurely at what would falsely appear to be an Empty slot. ✓

---

## Load-factor invariant and rehash

**Invariant:** After every public operation, `occupied_ / capacity ≤ kMaxLoadFactor` (0.75).

- `insert` checks `occupied_ + 1 > capacity * 0.75` *before* the insertion. If true, it rehashes to `2 × capacity` first. After rehash all tombstones are cleared (`occupied_ == size_`), so the new load is `size_ / (2 × old_cap) ≤ 0.375`.
- `erase` decrements `size_` but not `occupied_`, so tombstones accumulate. The invariant still holds because `occupied_` was already bounded before.
- `clear` resets both counters to zero. ✓

---

## Expected O(1) probe length

With a uniform hash and load factor α ≤ 0.75, the expected number of slots examined per unsuccessful probe is 1/(1 − α)² ≤ 16 (for α = 0.75). Expected successful probe length ≤ 1 + 1/(1 − α) / 2 ≈ 2.5. Both are O(1). ✓

---

## Amortised O(1) insert

Each `insert` either:
1. Updates an existing key (no `occupied_` change), or
2. Places a new entry. If the slot used was Deleted, `occupied_` is unchanged; if Empty, `occupied_` increases by 1.

A rehash doubles capacity and costs O(old_cap) = O(size). Using a standard amortised argument (potential = size), the total cost over n inserts is O(n), giving O(1) amortised per insert. ✓

---

## References

- D. Knuth, *The Art of Computer Programming Vol. 3*, §6.4 — open addressing analysis.
- T. H. Cormen et al., *Introduction to Algorithms* 4th ed., §11.4 — open addressing and load factor.
