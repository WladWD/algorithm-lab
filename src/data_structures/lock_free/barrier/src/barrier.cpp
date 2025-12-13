#include "data_structures/lock_free/barrier/barrier.h"

#include <thread>

namespace data_structures::lock_free {

namespace {

inline void cpu_relax() noexcept {
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
    asm volatile("pause" ::: "memory");
#else
    std::this_thread::yield();
#endif
}

} // namespace

Barrier::Barrier(std::size_t expected) noexcept
    : expected_{expected},
      arrived_{0},
      phase_{0} {}

void Barrier::arrive_and_wait() noexcept { arrive(false); }

void Barrier::arrive_and_drop() noexcept { arrive(true); }

void Barrier::arrive(bool drop) noexcept {
    const std::size_t current_phase = phase_.load(std::memory_order_acquire);
    const std::size_t expected = expected_.load(std::memory_order_acquire);

    // Order of operations:
    // 1. Increment arrival count for this phase.
    // 2. If this is the last arriving participant, advance the phase.
    const std::size_t prev = arrived_.fetch_add(1, std::memory_order_acq_rel);
    const std::size_t new_count = prev + 1;

    if (new_count == expected) {
        // Last thread in this phase.
        arrived_.store(0, std::memory_order_relaxed);

        if (drop) {
            // Reduce expected participants for future phases.
            expected_.fetch_sub(1, std::memory_order_acq_rel);
        }

        // Publish the next phase. Release ensures all prior writes from any
        // participant become visible to threads that observe the new phase
        // with acquire.
        phase_.store(current_phase + 1, std::memory_order_release);
        return;
    }

    // Not the last thread: wait until the phase changes.
    while (phase_.load(std::memory_order_acquire) == current_phase) {
        cpu_relax();
    }
}

CountDownLatch::CountDownLatch(std::size_t initial_count) noexcept
    : count_{initial_count} {}

void CountDownLatch::count_down(std::size_t n) noexcept {
    if (n == 0) {
        return;
    }

    // We allow the counter to underflow only conceptually; callers are
    // expected to respect the contract and not decrement past zero.
    count_.fetch_sub(n, std::memory_order_acq_rel);
}

void CountDownLatch::wait() const noexcept {
    while (!is_ready()) {
        backoff();
    }
}

bool CountDownLatch::is_ready() const noexcept {
    return count_.load(std::memory_order_acquire) == 0;
}

void CountDownLatch::backoff() noexcept {
    cpu_relax();
}

} // namespace data_structures::lock_free

