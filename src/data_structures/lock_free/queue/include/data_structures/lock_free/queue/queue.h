#pragma once

#include <atomic>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>

namespace data_structures::lock_free::queue {
// Michael & Scott lock-free queue (header-only, templated)
// NOTE: This implementation uses a per-thread retired-list registry to avoid
// a global mutex in the hot path. It is NOT a full production-grade
// reclamation scheme (hazard pointers / epochs are recommended for safety).
template <typename T> class LockFreeQueue {
  private:
    struct Node {
        std::optional<T> data;
        std::atomic<Node*> next{nullptr};

        Node() : data(std::nullopt), next(nullptr) {}
        explicit Node(T&& v) : data(std::move(v)), next(nullptr) {}
        explicit Node(const T& v) : data(v), next(nullptr) {}
    };

    std::atomic<Node*> head_{nullptr};
    std::atomic<Node*> tail_{nullptr};

    static inline std::mutex registry_mtx_{};
    static inline std::vector<std::vector<Node*>*> registry_{};

    static void retire_node(Node* n) {
        thread_local std::vector<Node*> local_retired;
        thread_local bool registered = ([]() {
            std::lock_guard<std::mutex> g(registry_mtx_);
            registry_.push_back(&local_retired);
            return true;
        })();
        (void)registered;
        local_retired.push_back(n);
    }

  public:
    LockFreeQueue() {
        Node* sentinel = new Node();
        head_.store(sentinel, std::memory_order_relaxed);
        tail_.store(sentinel, std::memory_order_relaxed);
    }

    ~LockFreeQueue() { clear(); }

    LockFreeQueue(const LockFreeQueue&) = delete;
    LockFreeQueue& operator=(const LockFreeQueue&) = delete;

    void push(const T& value) {
        Node* node = new Node(value);
        node->next.store(nullptr, std::memory_order_relaxed);

        while (true) {
            Node* last = tail_.load(std::memory_order_acquire);
            Node* next = last->next.load(std::memory_order_acquire);
            if (last == tail_.load(std::memory_order_acquire)) {
                if (next == nullptr) {
                    if (last->next.compare_exchange_weak(next, node, std::memory_order_release,
                                                         std::memory_order_relaxed)) {
                        tail_.compare_exchange_weak(last, node, std::memory_order_release,
                                                    std::memory_order_relaxed);
                        return;
                    }
                } else {
                    tail_.compare_exchange_weak(last, next, std::memory_order_release,
                                                std::memory_order_relaxed);
                }
            }
        }
    }

    void push(T&& value) {
        Node* node = new Node(std::move(value));
        node->next.store(nullptr, std::memory_order_relaxed);

        while (true) {
            Node* last = tail_.load(std::memory_order_acquire);
            Node* next = last->next.load(std::memory_order_acquire);
            if (last == tail_.load(std::memory_order_acquire)) {
                if (next == nullptr) {
                    if (last->next.compare_exchange_weak(next, node, std::memory_order_release,
                                                         std::memory_order_relaxed)) {
                        tail_.compare_exchange_weak(last, node, std::memory_order_release,
                                                    std::memory_order_relaxed);
                        return;
                    }
                } else {
                    tail_.compare_exchange_weak(last, next, std::memory_order_release,
                                                std::memory_order_relaxed);
                }
            }
        }
    }

    // Dequeue (pop). Returns std::nullopt if empty.
    std::optional<T> pop() {
        while (true) {
            Node* first = head_.load(std::memory_order_acquire);
            Node* last = tail_.load(std::memory_order_acquire);
            Node* next = first->next.load(std::memory_order_acquire);

            if (first == head_.load(std::memory_order_acquire)) {
                if (first == last) {
                    if (next == nullptr) {
                        return std::nullopt; // empty
                    }
                    tail_.compare_exchange_weak(last, next, std::memory_order_release,
                                                std::memory_order_relaxed);
                } else {
                    // Read value before CAS to avoid ABA on next
                    std::optional<T> result = next->data;
                    if (head_.compare_exchange_weak(first, next, std::memory_order_acq_rel,
                                                    std::memory_order_relaxed)) {
                        retire_node(first);
                        return result;
                    }
                }
            }
        }
    }

    // Non-atomic emptiness check (may be racy)
    bool empty() const noexcept {
        Node* first = head_.load(std::memory_order_acquire);
        Node* next = first->next.load(std::memory_order_acquire);
        return (next == nullptr);
    }

    // Reclaim all nodes (must only be called when no concurrent operations)
    void clear() {
        Node* cur = head_.exchange(nullptr, std::memory_order_acq_rel);
        if (cur) {
            Node* p = cur;
            while (p) {
                Node* nx = p->next.load(std::memory_order_relaxed);
                delete p;
                p = nx;
            }
        }

        std::lock_guard<std::mutex> g(registry_mtx_);
        for (auto vec_ptr : registry_) {
            if (vec_ptr) {
                for (Node* n : *vec_ptr)
                    delete n;
                vec_ptr->clear();
            }
        }

        Node* sentinel = new Node();
        head_.store(sentinel, std::memory_order_relaxed);
        tail_.store(sentinel, std::memory_order_relaxed);
    }
};
} // namespace data_structures::lock_free::queue
