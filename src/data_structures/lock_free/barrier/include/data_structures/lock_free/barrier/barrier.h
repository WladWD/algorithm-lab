#pragma once

#include <atomic>
#include <chrono>
#include <cstddef>

namespace data_structures::lock_free {
/// \brief Reusable barrier for a fixed number of participating threads.
///
/// The barrier is phase-based: each full round of arrivals advances an
/// internal generation counter. A call to arrive_and_wait() establishes a
/// happens-before relationship between all work done before the call in any
/// participating thread and all work done after the call in any participating
/// thread in the next phase.
class Barrier {
  public:
    /// Construct a barrier for \p expected participants.
    /// Behaviour is undefined if expected == 0.
    explicit Barrier(std::size_t expected) noexcept;

    Barrier(const Barrier&) = delete;
    Barrier& operator=(const Barrier&) = delete;

    /// Arrive at the barrier and block until all participants for the current
    /// phase have arrived.
    void arrive_and_wait() noexcept;

    /// Arrive at the barrier and indicate that this participant will not take
    /// part in subsequent phases. This decreases the number of expected
    /// participants for future phases by one when the current phase completes.
    void arrive_and_drop() noexcept;

  private:
    void arrive(bool drop) noexcept;

    std::atomic<std::size_t> expected_;
    std::atomic<std::size_t> arrived_;
    std::atomic<std::size_t> phase_;
};

/// \brief One-shot countdown latch.
///
/// Allows one or more waiting threads to block in wait() until the internal
/// counter reaches zero. Producers call count_down() to decrement the counter.
class CountDownLatch {
  public:
    /// Create a latch with the given initial count. Behaviour is undefined if
    /// initial_count is zero and wait() is called.
    explicit CountDownLatch(std::size_t initial_count) noexcept;

    CountDownLatch(const CountDownLatch&) = delete;
    CountDownLatch& operator=(const CountDownLatch&) = delete;

    /// Decrement the counter by n (default 1). When the counter reaches zero,
    /// all waiting threads are released.
    void count_down(std::size_t n = 1) noexcept;

    /// Block until the counter reaches zero.
    void wait() const noexcept;

    /// Timed wait: returns true if the latch reached zero before the timeout
    /// expired, false otherwise.
    template <class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& timeout) const noexcept {
        const auto deadline = std::chrono::steady_clock::now() + timeout;
        while (!is_ready()) {
            if (std::chrono::steady_clock::now() >= deadline) {
                return is_ready();
            }
            backoff();
        }
        return true;
    }

  private:
    bool is_ready() const noexcept;
    static void backoff() noexcept;

    mutable std::atomic<std::size_t> count_;
};
} // namespace data_structures::lock_free
