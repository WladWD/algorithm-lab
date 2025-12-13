#include "data_structures/lock_free/spinlock/spinlock.h"

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

void Spinlock::lock() noexcept {
    Backoff backoff;
    while (!try_lock()) {
        backoff.pause();
    }
}

bool Spinlock::try_lock() noexcept {
    // Test-and-set with acquire semantics; returns true on success.
    return !flag_.test_and_set(std::memory_order_acquire);
}

void Spinlock::unlock() noexcept {
    flag_.clear(std::memory_order_release);
}

void Backoff::pause() noexcept {
    // Simple two-phase strategy:
    //  - for the first few attempts, busy-wait with cpu_relax.
    //  - afterwards, yield to the OS scheduler to reduce contention.
    if (count_ < 16) {
        ++count_;
        cpu_relax();
    } else {
        std::this_thread::yield();
    }
}

} // namespace data_structures::lock_free

