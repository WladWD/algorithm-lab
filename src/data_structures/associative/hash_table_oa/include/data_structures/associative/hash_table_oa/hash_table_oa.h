#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>

namespace ds::hash_table_oa {

// Hash table using open addressing with linear probing.
//
// Collisions are resolved by probing consecutive slots (wrap-around).
// Deletion uses tombstone markers so subsequent probes over deleted slots
// continue correctly.
//
// Rehash policy: when size_ exceeds kMaxLoadFactor * capacity, a new table
// of 2× capacity is allocated and all live entries are re-inserted.
// After rehash all tombstones are cleared, keeping the expected probe
// length bounded by 1/(1-α) where α = size/capacity.
//
// All operations: O(1) expected amortized time.
// Space: O(capacity), capacity ≥ size / kMaxLoadFactor.

template <typename Key,
          typename Value,
          typename Hash     = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class HashTableOA {
public:
    explicit HashTableOA(std::size_t initial_capacity = 16);

    /// Insert or overwrite value for key. O(1) amortized.
    void insert(Key key, Value value);

    /// Remove key. Returns true if key was present. O(1) amortized.
    bool erase(const Key& key);

    /// Return pointer to value, nullptr if absent. O(1) amortized.
    Value*       find(const Key& key);
    const Value* find(const Key& key) const;

    /// True if key is present. O(1) amortized.
    bool contains(const Key& key) const;

    /// Insert default-constructed value if absent, then return reference.
    /// Requires Value to be default-constructible. O(1) amortized.
    Value& operator[](const Key& key);

    std::size_t size()        const noexcept { return size_; }
    bool        empty()       const noexcept { return size_ == 0; }
    std::size_t capacity()    const noexcept { return table_.size(); }
    double      load_factor() const noexcept {
        return table_.empty() ? 0.0 : static_cast<double>(size_) / table_.size();
    }

    void clear();

private:
    static constexpr double kMaxLoadFactor = 0.75;

    enum class State : std::uint8_t { Empty, Occupied, Deleted };

    struct Slot {
        std::pair<Key, Value> kv{};
        State                 state = State::Empty;
    };

    std::vector<Slot> table_;
    std::size_t       size_     = 0; // live entries
    std::size_t       occupied_ = 0; // live + tombstone slots
    Hash              hasher_{};
    KeyEqual          equal_{};

    // Return index of slot holding key, or table_.size() if not found.
    std::size_t find_slot(const Key& key) const {
        const std::size_t cap = table_.size();
        const std::size_t h   = hasher_(key) % cap;
        for (std::size_t i = 0; i < cap; ++i) {
            std::size_t idx = (h + i) % cap;
            if (table_[idx].state == State::Empty)                           return cap;
            if (table_[idx].state == State::Occupied &&
                equal_(table_[idx].kv.first, key))                           return idx;
        }
        return cap;
    }

    // Return index for insertion: existing-key slot (for update) or best
    // available slot (first tombstone encountered, else first empty).
    std::size_t insert_slot(const Key& key) const {
        const std::size_t cap       = table_.size();
        const std::size_t h         = hasher_(key) % cap;
        std::size_t       first_del = cap;
        for (std::size_t i = 0; i < cap; ++i) {
            std::size_t idx = (h + i) % cap;
            const Slot& s   = table_[idx];
            if (s.state == State::Occupied && equal_(s.kv.first, key)) return idx;
            if (s.state == State::Deleted  && first_del == cap)         first_del = idx;
            if (s.state == State::Empty)
                return first_del != cap ? first_del : idx;
        }
        return first_del; // only tombstones remain — use first one found
    }

    void rehash(std::size_t new_cap) {
        std::vector<Slot> old = std::move(table_);
        table_.assign(new_cap, Slot{});
        size_ = occupied_ = 0;
        for (auto& s : old)
            if (s.state == State::Occupied)
                insert(std::move(s.kv.first), std::move(s.kv.second));
    }
};

// ──────────────────────────── method definitions ────────────────────────────

template <typename K, typename V, typename H, typename E>
HashTableOA<K, V, H, E>::HashTableOA(std::size_t initial_capacity)
    : table_(initial_capacity < 1 ? 1 : initial_capacity) {}

template <typename K, typename V, typename H, typename E>
void HashTableOA<K, V, H, E>::insert(K key, V value) {
    // Pessimistically assume a new empty slot will be consumed; rehash first.
    if (occupied_ + 1 > static_cast<std::size_t>(table_.size() * kMaxLoadFactor))
        rehash(table_.size() * 2);

    std::size_t idx  = insert_slot(key);
    Slot&       slot = table_[idx];

    if (slot.state == State::Occupied) {
        slot.kv.second = std::move(value); // update existing key
        return;
    }
    bool was_empty = (slot.state == State::Empty);
    slot.kv        = {std::move(key), std::move(value)};
    slot.state     = State::Occupied;
    ++size_;
    if (was_empty) ++occupied_; // tombstones already counted in occupied_
}

template <typename K, typename V, typename H, typename E>
bool HashTableOA<K, V, H, E>::erase(const K& key) {
    std::size_t idx = find_slot(key);
    if (idx == table_.size()) return false;
    table_[idx].state = State::Deleted;
    --size_;
    return true;
}

template <typename K, typename V, typename H, typename E>
V* HashTableOA<K, V, H, E>::find(const K& key) {
    std::size_t idx = find_slot(key);
    return idx == table_.size() ? nullptr : &table_[idx].kv.second;
}

template <typename K, typename V, typename H, typename E>
const V* HashTableOA<K, V, H, E>::find(const K& key) const {
    std::size_t idx = find_slot(key);
    return idx == table_.size() ? nullptr : &table_[idx].kv.second;
}

template <typename K, typename V, typename H, typename E>
bool HashTableOA<K, V, H, E>::contains(const K& key) const {
    return find_slot(key) != table_.size();
}

template <typename K, typename V, typename H, typename E>
V& HashTableOA<K, V, H, E>::operator[](const K& key) {
    if (!contains(key)) insert(key, V{});
    return *find(key);
}

template <typename K, typename V, typename H, typename E>
void HashTableOA<K, V, H, E>::clear() {
    for (auto& s : table_) s.state = State::Empty;
    size_ = occupied_ = 0;
}

} // namespace ds::hash_table_oa
