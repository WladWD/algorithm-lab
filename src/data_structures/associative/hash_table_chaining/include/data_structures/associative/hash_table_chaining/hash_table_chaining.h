#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace ds::hash_table_chaining {

// Hash table using separate chaining for collision resolution.
//
// Each bucket holds a contiguous list of (key, value) pairs.
// On collision, the new entry is appended to the bucket's list.
// Lookup and erase scan the bucket linearly — O(chain length) worst
// case, O(1) expected when the load factor is bounded.
//
// Rehash policy: when size_ exceeds kMaxLoadFactor * capacity, a new
// bucket array of 2× capacity is allocated and all entries re-hashed.
//
// All operations: O(1) expected amortized time.
// Space: O(capacity + size).

template <typename Key,
          typename Value,
          typename Hash     = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class HashTableChaining {
public:
    explicit HashTableChaining(std::size_t initial_capacity = 16);

    /// Insert or overwrite value for key. O(1) expected amortized.
    void insert(Key key, Value value);

    /// Remove key. Returns true if key was present. O(1) expected.
    bool erase(const Key& key);

    /// Return pointer to value, nullptr if absent. O(1) expected.
    Value*       find(const Key& key);
    const Value* find(const Key& key) const;

    /// True if key is present. O(1) expected.
    bool contains(const Key& key) const;

    /// Insert default-constructed value if absent, return reference.
    /// Requires Value to be default-constructible. O(1) expected amortized.
    Value& operator[](const Key& key);

    std::size_t size()        const noexcept { return size_; }
    bool        empty()       const noexcept { return size_ == 0; }
    std::size_t capacity()    const noexcept { return buckets_.size(); }
    double      load_factor() const noexcept {
        return buckets_.empty() ? 0.0 : static_cast<double>(size_) / buckets_.size();
    }

    void clear();

private:
    static constexpr double kMaxLoadFactor = 1.0;

    using Bucket = std::vector<std::pair<Key, Value>>;

    std::vector<Bucket> buckets_;
    std::size_t         size_ = 0;
    Hash                hasher_{};
    KeyEqual            equal_{};

    Bucket& bucket_for(const Key& key) {
        return buckets_[hasher_(key) % buckets_.size()];
    }
    const Bucket& bucket_for(const Key& key) const {
        return buckets_[hasher_(key) % buckets_.size()];
    }

    void rehash(std::size_t new_cap) {
        std::vector<Bucket> old = std::move(buckets_);
        buckets_.assign(new_cap, Bucket{});
        size_ = 0;
        for (auto& b : old)
            for (auto& kv : b)
                insert(std::move(kv.first), std::move(kv.second));
    }
};

// ──────────────────────────── method definitions ────────────────────────────

template <typename K, typename V, typename H, typename E>
HashTableChaining<K, V, H, E>::HashTableChaining(std::size_t initial_capacity)
    : buckets_(initial_capacity < 1 ? 1 : initial_capacity) {}

template <typename K, typename V, typename H, typename E>
void HashTableChaining<K, V, H, E>::insert(K key, V value) {
    if (size_ + 1 > static_cast<std::size_t>(buckets_.size() * kMaxLoadFactor))
        rehash(buckets_.size() * 2);

    Bucket& b = bucket_for(key);
    for (auto& kv : b) {
        if (equal_(kv.first, key)) {
            kv.second = std::move(value); // update
            return;
        }
    }
    b.emplace_back(std::move(key), std::move(value));
    ++size_;
}

template <typename K, typename V, typename H, typename E>
bool HashTableChaining<K, V, H, E>::erase(const K& key) {
    Bucket& b = bucket_for(key);
    for (auto it = b.begin(); it != b.end(); ++it) {
        if (equal_(it->first, key)) {
            b.erase(it);
            --size_;
            return true;
        }
    }
    return false;
}

template <typename K, typename V, typename H, typename E>
V* HashTableChaining<K, V, H, E>::find(const K& key) {
    for (auto& kv : bucket_for(key))
        if (equal_(kv.first, key)) return &kv.second;
    return nullptr;
}

template <typename K, typename V, typename H, typename E>
const V* HashTableChaining<K, V, H, E>::find(const K& key) const {
    for (const auto& kv : bucket_for(key))
        if (equal_(kv.first, key)) return &kv.second;
    return nullptr;
}

template <typename K, typename V, typename H, typename E>
bool HashTableChaining<K, V, H, E>::contains(const K& key) const {
    return find(key) != nullptr;
}

template <typename K, typename V, typename H, typename E>
V& HashTableChaining<K, V, H, E>::operator[](const K& key) {
    if (!contains(key)) insert(key, V{});
    return *find(key);
}

template <typename K, typename V, typename H, typename E>
void HashTableChaining<K, V, H, E>::clear() {
    for (auto& b : buckets_) b.clear();
    size_ = 0;
}

} // namespace ds::hash_table_chaining
