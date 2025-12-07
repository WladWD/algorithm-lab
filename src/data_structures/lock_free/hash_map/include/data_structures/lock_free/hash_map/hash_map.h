#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace data_structures::lock_free::hash_map {

// A simple, minimal lock-free hash map with separate chaining.
// - Fixed number of buckets provided at construction.
// - Insert prepends a node via CAS to the bucket head.
// - Erase performs logical deletion by setting an atomic flag on the node.
// - Find skips logically deleted nodes.
// Memory reclamation is NOT provided: deleted nodes are retained until clear()
// which must only be called when there are no concurrent operations.

template <typename K, typename V>
class LockFreeHashMap {
private:
    struct Node {
        K key;
        V value;
        std::atomic<bool> deleted{false};
        Node* next{nullptr};

        Node(const K& k, const V& v) : key(k), value(v), deleted(false), next(nullptr) {}
        Node(K&& k, V&& v) : key(std::move(k)), value(std::move(v)), deleted(false), next(nullptr) {}
    };

    const size_t buckets_;
    std::vector<std::atomic<Node*>> table_;
    std::hash<K> hasher_;
    std::atomic<size_t> size_{0};

    // Simple registry for nodes that can be freed on clear(); not used for per-op reclamation
    static inline std::mutex registry_mtx_{};
    static inline std::vector<Node*> registry_{};
    static void retire_node(Node* n) {
        std::lock_guard<std::mutex> g(registry_mtx_);
        registry_.push_back(n);
    }

    size_t bucket_index(const K& key) const noexcept { return hasher_(key) % buckets_; }

  public:
    explicit LockFreeHashMap(size_t buckets = 1024) : buckets_(buckets), table_(buckets) {
        for (size_t i = 0; i < buckets_; ++i)
            table_[i].store(nullptr, std::memory_order_relaxed);
    }

    ~LockFreeHashMap() { clear(); }

    LockFreeHashMap(const LockFreeHashMap&) = delete;
    LockFreeHashMap& operator=(const LockFreeHashMap&) = delete;

    // Insert only if key does not exist. Returns true if inserted, false if key already present.
    bool insert(const K& key, const V& value) {
        size_t idx = bucket_index(key);
        Node* new_node = new Node(key, value);

        while (true) {
            Node* head = table_[idx].load(std::memory_order_acquire);
            // check if key already present (and not deleted)
            for (Node* p = head; p != nullptr; p = p->next) {
                if (!p->deleted.load(std::memory_order_acquire) && p->key == key) {
                    delete new_node;
                    return false;
                }
            }

            new_node->next = head;
            if (table_[idx].compare_exchange_weak(head, new_node, std::memory_order_release,
                                                  std::memory_order_relaxed)) {
                size_.fetch_add(1, std::memory_order_relaxed);
                return true;
            }
            // CAS failed: retry. Key check will run again on new head.
        }
    }

    bool insert(K&& key, V&& value) {
        size_t idx = bucket_index(key);
        Node* new_node = new Node(std::move(key), std::move(value));

        while (true) {
            Node* head = table_[idx].load(std::memory_order_acquire);
            for (Node* p = head; p != nullptr; p = p->next) {
                if (!p->deleted.load(std::memory_order_acquire) && p->key == new_node->key) {
                    delete new_node;
                    return false;
                }
            }
            new_node->next = head;
            if (table_[idx].compare_exchange_weak(head, new_node, std::memory_order_release,
                                                  std::memory_order_relaxed)) {
                size_.fetch_add(1, std::memory_order_relaxed);
                return true;
            }
        }
    }

    // Find returns a copy of the value if present, std::nullopt otherwise.
    std::optional<V> find(const K& key) const {
        size_t idx = bucket_index(key);
        Node* head = table_[idx].load(std::memory_order_acquire);
        for (Node* p = head; p != nullptr; p = p->next) {
            if (!p->deleted.load(std::memory_order_acquire) && p->key == key) {
                return p->value;
            }
        }
        return std::nullopt;
    }

    // Logical erase. Returns true if a live node was marked deleted.
    bool erase(const K& key) {
        size_t idx = bucket_index(key);
        Node* head = table_[idx].load(std::memory_order_acquire);
        for (Node* p = head; p != nullptr; p = p->next) {
            bool expected = false;
            if (!p->deleted.load(std::memory_order_acquire) && p->key == key) {
                if (p->deleted.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
                    size_.fetch_sub(1, std::memory_order_relaxed);
                    // We don't physically remove the node to keep the implementation simple.
                    // Optionally retire the node for later deletion.
                    retire_node(p);
                    return true;
                } else {
                    return false; // already deleted concurrently
                }
            }
        }
        return false;
    }

    size_t size() const noexcept { return size_.load(std::memory_order_relaxed); }

    bool empty() const noexcept { return size() == 0; }

    // Clear the table. Must only be called when there are no concurrent ops.
    void clear() {
        for (size_t i = 0; i < buckets_; ++i) {
            Node* cur = table_[i].exchange(nullptr, std::memory_order_acq_rel);
            Node* p = cur;
            while (p) {
                Node* nx = p->next;
                delete p;
                p = nx;
            }
        }

        std::lock_guard<std::mutex> g(registry_mtx_);
        for (auto n : registry_) delete n;
        registry_.clear();
        size_.store(0, std::memory_order_relaxed);
    }
};
} // namespace data_structures::lock_free::hash_map

