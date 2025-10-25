#pragma once

#include <atomic>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <mutex>

namespace data_structures::lock_free::stack {

// Treiber-style lock-free stack.
// NOTE: This implementation intentionally does NOT provide a safe
// memory-reclamation scheme (hazard pointers / epoch reclamation).
// That means nodes removed by concurrent pop() are not reclaimed
// in a thread-safe manner by default. Use `clear()` only when you
// can guarantee no concurrent operations (single-threaded teardown),
// or integrate a proper reclamation scheme for production use.

template <typename T>
class LockFreeStack {
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

  // Push a copy of value onto the stack.
  void push(const T& value) {
    Node* node = new Node(value);
    node->next = head_.load(std::memory_order_relaxed);
    while (!head_.compare_exchange_weak(node->next, node,
                                        std::memory_order_release,
                                        std::memory_order_relaxed)) {
      // loop until successful
    }
  }

  // Push by move
  void push(T&& value) {
    Node* node = new Node(std::move(value));
    node->next = head_.load(std::memory_order_relaxed);
    while (!head_.compare_exchange_weak(node->next, node,
                                        std::memory_order_release,
                                        std::memory_order_relaxed)) {
      // retry
    }
  }

  // Emplace convenience
  template <typename... Args>
  void emplace(Args&&... args) {
    Node* node = new Node(std::forward<Args>(args)...);
    node->next = head_.load(std::memory_order_relaxed);
    while (!head_.compare_exchange_weak(node->next, node,
                                        std::memory_order_release,
                                        std::memory_order_relaxed)) {
      // retry
    }
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
      if (head_.compare_exchange_weak(old_head, next,
                                      std::memory_order_acquire,
                                      std::memory_order_relaxed)) {
        // Successfully removed old_head from the stack.
        // Move out the data for the caller.
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
  bool empty() const noexcept {
    return head_.load(std::memory_order_acquire) == nullptr;
  }

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
      for (Node* n : retired_) delete n;
      retired_.clear();
    }
  }

  // For debugging / instrumentation only: approximate number of nodes
  // currently in the stack (may be expensive and racy).
  size_t approximate_size() const {
    size_t cnt = 0;
    Node* cur = head_.load(std::memory_order_acquire);
    while (cur) {
      ++cnt;
      cur = cur->next;
    }
    return cnt;
  }
};

} // namespace data_structures::lock_free::stack