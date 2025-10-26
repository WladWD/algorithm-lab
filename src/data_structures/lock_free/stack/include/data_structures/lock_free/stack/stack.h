#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>
#include <stack>
#include <utility>
#include <vector>

namespace data_structures::lock_free::stack {
// Treiber-style lock-free stack. https://en.wikipedia.org/wiki/Treiber_stack
// NOTE: This implementation intentionally does NOT provide a safe
// memory-reclamation scheme (hazard pointers / epoch reclamation).
// That means nodes removed by concurrent pop() are not reclaimed
// in a thread-safe manner by default. Use `clear()` only when you
// can guarantee no concurrent operations (single-threaded teardown),
// or integrate a proper reclamation scheme for production use.
// https://en.wikipedia.org/wiki/ABA_problem
template <typename T> class LockFreeStack {
  private:
    struct Node {
        T data;
        Node* next;

        template <typename... Args>
        explicit Node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr) {}
    };

    std::atomic<Node*> head_{nullptr};

    // Optional pool of retired nodes that can be reclaimed by the owner
    // when it is safe to do so (single-threaded). Access to retired_ is
    // guarded by retired_mtx_ to allow a safe clear() in single-threaded
    // teardown scenarios.
    std::vector<Node*> retired_;
    std::mutex retired_mtx_;

  public:
    LockFreeStack() noexcept = default;
    ~LockFreeStack() {
        // Danger: only safe to call if no other threads may access the stack.
        // We try to reclaim nodes, but this destructor is NOT safe if other
        // threads are concurrently operating on the stack.
        clear();
    }

    LockFreeStack(const LockFreeStack&) = delete;
    LockFreeStack& operator=(const LockFreeStack&) = delete;

    void push(const T& value) {
        Node* node = new Node(value);
        node->next = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(node->next, node, std::memory_order_release,
                                            std::memory_order_relaxed))
            ;
    }

    void push(T&& value) {
        Node* node = new Node(std::move(value));
        node->next = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(node->next, node, std::memory_order_release,
                                            std::memory_order_relaxed))
            ;
    }

    template <typename... Args> void emplace(Args&&... args) {
        Node* node = new Node(std::forward<Args>(args)...);
        node->next = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(node->next, node, std::memory_order_release,
                                            std::memory_order_relaxed))
            ;
    }

    // Pop an element. Returns std::nullopt if stack was empty.
    // The returned T is copied/moved out of the node.
    // WARNING: This implementation does not reclaim Node memory in a
    // thread-safe way. Nodes are added to an internal retired list and
    // reclaimed only by clear() or destructor when it is safe (no other
    // concurrent access).
    std::optional<T> pop() {
        Node* old_head = head_.load(std::memory_order_acquire);
        while (old_head != nullptr) {
            Node* next = old_head->next;
            if (head_.compare_exchange_weak(old_head, next, std::memory_order_acquire,
                                            std::memory_order_relaxed)) {
                T value = std::move(old_head->data);

                // Defer reclamation: store the node for later reclamation.
                // This avoids immediate deletion which would be unsafe without
                // a proper memory-reclamation scheme.
                {
                    std::lock_guard<std::mutex> g(retired_mtx_);
                    retired_.push_back(old_head);
                }

                return std::optional<T>(std::move(value));
            }
            // old_head was updated by another thread; retry with new value
        }
        return std::nullopt;
    }

    // Non-atomic check for emptiness (may be racy).
    bool empty() const noexcept { return head_.load(std::memory_order_acquire) == nullptr; }

    // Reclaim all retired nodes. This must be called only when no other
    // threads will access the stack (single-threaded teardown) or when
    // a proper synchronization/hand-off protocol is used.
    void clear() {
        Node* cur = head_.exchange(nullptr, std::memory_order_acq_rel);
        // Delete remaining nodes from the stack
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        // Delete retired nodes
        {
            std::lock_guard<std::mutex> g(retired_mtx_);
            for (Node* n : retired_) {
                delete n;
            }
            retired_.clear();
        }
    }
};

template <typename T> class LockBasedStack {
  private:
    std::stack<T> stack;
    mutable std::mutex accessMutex;

  public:
    LockBasedStack() noexcept = default;
    ~LockBasedStack() {
        std::lock_guard<std::mutex> guard(accessMutex);
        while (!stack.empty()) {
            stack.pop();
        }
    }

    LockBasedStack(const LockBasedStack&) = delete;
    LockBasedStack& operator=(const LockBasedStack&) = delete;

    void push(const T& value) {
        std::lock_guard<std::mutex> guard(accessMutex);
        stack.push(value);
    }

    void push(T&& value) {
        std::lock_guard<std::mutex> guard(accessMutex);
        stack.push(std::move(value));
    }

    template <typename... Args> void emplace(Args&&... args) {
        std::lock_guard<std::mutex> guard(accessMutex);
        stack.push(T(std::forward<Args>(args)...));
    }

    std::optional<T> pop() {
        std::lock_guard<std::mutex> guard(accessMutex);
        if (stack.empty()) {
            return std::nullopt;
        }

        T value = std::move(stack.top());
        stack.pop();
        return std::optional<T>(std::move(value));
    }

    bool empty() const noexcept {
        std::lock_guard guard(accessMutex);
        return stack.empty();
    }
};
} // namespace data_structures::lock_free::stack