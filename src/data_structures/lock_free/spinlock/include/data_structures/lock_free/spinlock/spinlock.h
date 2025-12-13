#pragma once

#include <atomic>

namespace data_structures::lock_free {

// Simple spinlock suitable for very short critical sections.
//
// This implementation uses std::atomic_flag with acquire/release semantics
// and an internal backoff loop to reduce contention.
class Spinlock {
public:
    Spinlock() noexcept : flag_{ATOMIC_FLAG_INIT} {}

    Spinlock(const Spinlock&) = delete;
    Spinlock& operator=(const Spinlock&) = delete;

    // Block until the lock is acquired.
    void lock() noexcept;

    // Try to acquire the lock without blocking. Returns true on success.
    bool try_lock() noexcept;

    // Release the lock.
    void unlock() noexcept;

private:
    std::atomic_flag flag_;
};

// Simple backoff helper suitable for use in spin loops.
//
// On early iterations it performs CPU relax hints; on later iterations it
// yields to the OS scheduler to reduce contention.
class Backoff {
public:
    Backoff() noexcept : count_{0} {}

    // Apply one step of backoff.
    void pause() noexcept;

private:
    unsigned count_;
};

} // namespace data_structures::lock_free

