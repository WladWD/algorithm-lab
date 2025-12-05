#pragma once

#include <atomic>
#include <array>
#include <cstddef>
#include <cstdint>
#include <new>
#include <thread>
#include <type_traits>
#include <vector>
#include <cassert>
#include <utility>

namespace data_structures::lock_free {

// Utility: round up to next power of two
inline size_t next_pow2(size_t v) {
    if (v <= 1) return 1;
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
#if SIZE_MAX > 0xffffffffu
    v |= v >> 32;
#endif
    return ++v;
}

// ---------------------- SPSC ring buffer ----------------------
// Single-producer / single-consumer ring buffer
// - Capacity is rounded up to power-of-two
// - Uses one-slot-reserved convention (usable capacity = cap - 1)
// - Non-blocking try_enqueue / try_dequeue

template<typename T>
class SpscRingBuffer {
public:
    explicit SpscRingBuffer(size_t capacity)
    : capacity_(next_pow2(std::max<size_t>(2, capacity))), mask_(capacity_ - 1), buffer_(capacity_)
    {
        head_.store(0, std::memory_order_relaxed);
        tail_.store(0, std::memory_order_relaxed);
    }

    ~SpscRingBuffer() {
        // destroy remaining elements if any
        size_t head = head_.load(std::memory_order_relaxed);
        size_t tail = tail_.load(std::memory_order_relaxed);
        while (head != tail) {
            T* p = elem_ptr(head);
            p->~T();
            head = (head + 1) & mask_;
        }
    }

    SpscRingBuffer(const SpscRingBuffer&) = delete;
    SpscRingBuffer& operator=(const SpscRingBuffer&) = delete;

    // Try to enqueue a copy; returns false if full.
    bool try_enqueue(const T& v) {
        const size_t tail = tail_.load(std::memory_order_relaxed);
        const size_t next = (tail + 1) & mask_;
        if (next == head_.load(std::memory_order_acquire)) return false; // full
        T* dst = elem_ptr(tail);
        new (dst) T(v);
        tail_.store(next, std::memory_order_release);
        return true;
    }

    // Try to enqueue by move
    bool try_enqueue(T&& v) {
        const size_t tail = tail_.load(std::memory_order_relaxed);
        const size_t next = (tail + 1) & mask_;
        if (next == head_.load(std::memory_order_acquire)) return false; // full
        T* dst = elem_ptr(tail);
        new (dst) T(std::move(v));
        tail_.store(next, std::memory_order_release);
        return true;
    }

    // Try to dequeue into out; returns false if empty
    bool try_dequeue(T& out) {
        const size_t head = head_.load(std::memory_order_relaxed);
        if (head == tail_.load(std::memory_order_acquire)) return false; // empty
        T* src = elem_ptr(head);
        out = std::move(*src);
        src->~T();
        head_.store((head + 1) & mask_, std::memory_order_release);
        return true;
    }

    size_t capacity() const noexcept { return capacity_; }
    size_t usable_capacity() const noexcept { return capacity_ - 1; }
    // approximate size (may race with concurrent ops)
    size_t size() const noexcept {
        size_t head = head_.load(std::memory_order_acquire);
        size_t tail = tail_.load(std::memory_order_acquire);
        if (tail >= head) return tail - head;
        return capacity_ - (head - tail);
    }

private:
    inline T* elem_ptr(size_t idx) noexcept {
        // placement location inside buffer_ which stores aligned byte arrays
        void* ptr = static_cast<void*>(buffer_[idx].data.data());
        return std::launder(reinterpret_cast<T*>(ptr));
    }

    const size_t capacity_;
    const size_t mask_;
    // storage: vector of uninitialized bytes aligned for T
    struct AlignedSlot { alignas(alignof(T)) std::array<std::byte, sizeof(T)> data; };
    std::vector<AlignedSlot> buffer_;
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> head_;
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> tail_;
};

// ---------------------- MPSC ring buffer (sketch / sequence-based) ----------------------
// Multiple producers, single consumer. Implementation follows the sequence-slot pattern.
// Note: try_enqueue will spin until the slot becomes available. This is a pragmatic,
// high-throughput approach but it is not a non-blocking try that returns false when full.

template<typename T>
class MpscRingBuffer {
public:
    static_assert(std::is_nothrow_move_constructible_v<T> || std::is_nothrow_copy_constructible_v<T>,
                  "MpscRingBuffer requires T to be nothrow-move-constructible or nothrow-copy-constructible;"
                  " throwing constructors can lead to consumer stalls/deadlock.");

    explicit MpscRingBuffer(size_t capacity)
    : capacity_(next_pow2(std::max<size_t>(2, capacity))), mask_(capacity_ - 1), slots_(capacity_)
    {
        // initialize per-slot sequence
        for (size_t i = 0; i < capacity_; ++i) {
            slots_[i].seq.store(i, std::memory_order_relaxed);
        }
        prod_idx_.store(0, std::memory_order_relaxed);
        cons_idx_.store(0, std::memory_order_relaxed);
    }

    ~MpscRingBuffer() {
        // consumer should have drained and destroyed elements; best-effort cleanup
    }

    MpscRingBuffer(const MpscRingBuffer&) = delete;
    MpscRingBuffer& operator=(const MpscRingBuffer&) = delete;

    // Enqueue (may spin until slot is available). Returns true on success.
    bool enqueue(const T& v) {
        uint64_t ticket = prod_idx_.fetch_add(1, std::memory_order_relaxed);
        Slot& slot = slots_[ticket & mask_];
        // wait until slot.seq == ticket
        uint64_t expected = ticket;
        while (slot.seq.load(std::memory_order_acquire) != expected) {
            // spin; in high contention consider pause or backoff
            std::this_thread::yield();
        }
        // construct in place
        void* dst = static_cast<void*>(slot.storage.data());
        new (dst) T(v);
        // publish
        slot.seq.store(ticket + 1, std::memory_order_release);
        return true;
    }

    bool enqueue(T&& v) {
        uint64_t ticket = prod_idx_.fetch_add(1, std::memory_order_relaxed);
        Slot& slot = slots_[ticket & mask_];
        uint64_t expected = ticket;
        while (slot.seq.load(std::memory_order_acquire) != expected) {
            std::this_thread::yield();
        }
        void* dst = static_cast<void*>(slot.storage.data());
        new (dst) T(std::move(v));
        slot.seq.store(ticket + 1, std::memory_order_release);
        return true;
    }

    // Try dequeue: must be called by single consumer.
    bool try_dequeue(T& out) {
        uint64_t cid = cons_idx_.load(std::memory_order_relaxed);
        Slot& slot = slots_[cid & mask_];
        uint64_t seq = slot.seq.load(std::memory_order_acquire);
        if (seq == cid + 1) {
            // ready
            T* ptr = std::launder(reinterpret_cast<T*>(static_cast<void*>(slot.storage.data())));
            out = std::move(*ptr);
            ptr->~T();
            slot.seq.store(cid + capacity_, std::memory_order_release); // make slot available; set to next expected ticket
            cons_idx_.store(cid + 1, std::memory_order_relaxed);
            return true;
        }
        return false;
    }

    size_t capacity() const noexcept { return capacity_; }

private:
    struct Slot {
        std::atomic<uint64_t> seq; // sequence/epoch
        // storage aligned for T using byte array
        alignas(alignof(T)) std::array<std::byte, sizeof(T)> storage;
    };

    const size_t capacity_;
    const uint64_t mask_;
    std::vector<Slot> slots_;
    alignas(std::hardware_destructive_interference_size) std::atomic<uint64_t> prod_idx_;
    alignas(std::hardware_destructive_interference_size) std::atomic<uint64_t> cons_idx_;
};

// ---------------------- MPMC ring buffer (Vyukov-style) ----------------------
// Multiple-producer / multiple-consumer lock-free bounded queue.
// - Non-blocking try_enqueue / try_dequeue that return false on full/empty
// - Uses sequence numbers per slot and CAS on head/tail indices
// - Power-of-two capacity for masking

template<typename T>
class MpmcRingBuffer {
public:
    static_assert(std::is_nothrow_move_constructible_v<T> || std::is_nothrow_copy_constructible_v<T>,
                  "MpmcRingBuffer requires T to be nothrow-move-constructible or nothrow-copy-constructible;") ;

    explicit MpmcRingBuffer(size_t capacity)
    : capacity_(next_pow2(std::max<size_t>(2, capacity))), mask_(capacity_ - 1), slots_(capacity_)
    {
        for (size_t i = 0; i < capacity_; ++i) {
            slots_[i].seq.store(i, std::memory_order_relaxed);
        }
        head_.store(0, std::memory_order_relaxed);
        tail_.store(0, std::memory_order_relaxed);
    }

    MpmcRingBuffer(const MpmcRingBuffer&) = delete;
    MpmcRingBuffer& operator=(const MpmcRingBuffer&) = delete;

    ~MpmcRingBuffer() {
        // best-effort: consumer should have drained queue; otherwise objects still in slots are leaked
    }

    // Non-blocking enqueue: returns false if queue appears full
    bool try_enqueue(const T& v) {
        uint64_t pos = tail_.load(std::memory_order_relaxed);
        while (true) {
            Slot& slot = slots_[pos & mask_];
            uint64_t seq = slot.seq.load(std::memory_order_acquire);
            intptr_t dif = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);
            if (dif == 0) {
                // try to claim this slot
                if (tail_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
                    void* dst = static_cast<void*>(slot.storage.data());
                    new (dst) T(v);
                    slot.seq.store(pos + 1, std::memory_order_release);
                    return true;
                }
                // CAS failed, pos updated with latest tail; retry
                continue;
            } else if (dif < 0) {
                // queue full
                return false;
            } else {
                // slot not ready yet, advance pos and retry
                pos = tail_.load(std::memory_order_relaxed);
            }
        }
    }

    bool try_enqueue(T&& v) {
        uint64_t pos = tail_.load(std::memory_order_relaxed);
        while (true) {
            Slot& slot = slots_[pos & mask_];
            uint64_t seq = slot.seq.load(std::memory_order_acquire);
            intptr_t dif = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);
            if (dif == 0) {
                if (tail_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
                    void* dst = static_cast<void*>(slot.storage.data());
                    new (dst) T(std::move(v));
                    slot.seq.store(pos + 1, std::memory_order_release);
                    return true;
                }
                continue;
            } else if (dif < 0) {
                return false;
            } else {
                pos = tail_.load(std::memory_order_relaxed);
            }
        }
    }

    // Non-blocking dequeue: returns false if queue appears empty
    bool try_dequeue(T& out) {
        uint64_t pos = head_.load(std::memory_order_relaxed);
        while (true) {
            Slot& slot = slots_[pos & mask_];
            uint64_t seq = slot.seq.load(std::memory_order_acquire);
            intptr_t dif = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos + 1);
            if (dif == 0) {
                if (head_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
                    T* ptr = std::launder(reinterpret_cast<T*>(static_cast<void*>(slot.storage.data())));
                    out = std::move(*ptr);
                    ptr->~T();
                    slot.seq.store(pos + capacity_, std::memory_order_release);
                    return true;
                }
                continue;
            } else if (dif < 0) {
                return false; // empty
            } else {
                pos = head_.load(std::memory_order_relaxed);
            }
        }
    }

    size_t capacity() const noexcept { return capacity_; }

private:
    struct Slot {
        std::atomic<uint64_t> seq;
        alignas(alignof(T)) std::array<std::byte, sizeof(T)> storage;
    };

    const size_t capacity_;
    const uint64_t mask_;
    std::vector<Slot> slots_;
    alignas(std::hardware_destructive_interference_size) std::atomic<uint64_t> head_;
    alignas(std::hardware_destructive_interference_size) std::atomic<uint64_t> tail_;
};

} // namespace data_structures::lock_free
