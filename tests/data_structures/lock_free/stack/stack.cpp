#include "data_structures/lock_free/stack/stack.h"

#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <numeric>
#include <thread>
#include <vector>

using namespace data_structures::lock_free::stack;

template <typename StackT>
void run_producer_consumer_test(size_t producers, size_t consumers, size_t per_producer) {
    StackT st;
    const size_t total = producers * per_producer;

    std::atomic<size_t> produced{0};
    std::atomic<size_t> consumed{0};
    std::atomic<long long> sum{0};

    // Launch producer threads
    std::vector<std::thread> prod_threads;
    prod_threads.reserve(producers);
    for (size_t p = 0; p < producers; ++p) {
        prod_threads.emplace_back([p, per_producer, &st, &produced]() {
            size_t base = p * per_producer;
            for (size_t i = 0; i < per_producer; ++i) {
                st.push(static_cast<int>(base + i));
                produced.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // Launch consumer threads
    std::vector<std::thread> cons_threads;
    cons_threads.reserve(consumers);
    for (size_t c = 0; c < consumers; ++c) {
        cons_threads.emplace_back([&st, &consumed, &sum, total]() {
            while (consumed.load(std::memory_order_relaxed) < total) {
                auto v = st.pop();
                if (v) {
                    sum.fetch_add(static_cast<long long>(*v), std::memory_order_relaxed);
                    consumed.fetch_add(1, std::memory_order_relaxed);
                } else {
                    // No value; yield briefly to let producers make progress
                    std::this_thread::yield();
                }
            }
        });
    }

    // Wait for producers to finish
    for (auto& t : prod_threads)
        t.join();

    // Wait for consumers to finish
    for (auto& t : cons_threads)
        t.join();

    ASSERT_EQ(produced.load(), total);
    ASSERT_EQ(consumed.load(), total);

    // Verify sum of consumed values equals expected arithmetic series sum
    // Expected sum = sum_{p=0..producers-1} sum_{i=0..per_producer-1} (p*per_producer + i)
    long long expected_sum = 0;
    for (size_t p = 0; p < producers; ++p) {
        long long base = static_cast<long long>(p * per_producer);
        // sum base + i for i in [0, per_producer)
        expected_sum +=
            per_producer * base + (static_cast<long long>(per_producer) * (per_producer - 1)) / 2;
    }

    ASSERT_EQ(sum.load(), expected_sum);

    // Stack should be empty now
    ASSERT_TRUE(st.empty());
}

// Single-threaded correctness test for LockFreeStack
TEST(LockFreeStackTests, SingleThreadedPushPop) {
    LockFreeStack<int> s;
    const int N = 1000;
    for (int i = 0; i < N; ++i)
        s.push(i);

    std::vector<bool> seen(N, false);
    int cnt = 0;
    while (true) {
        auto v = s.pop();
        if (!v)
            break;
        int x = *v;
        ASSERT_GE(x, 0);
        ASSERT_LT(x, N);
        seen[x] = true;
        ++cnt;
    }
    ASSERT_EQ(cnt, N);
    for (int i = 0; i < N; ++i)
        ASSERT_TRUE(seen[i]);
}

// Single-threaded correctness test for LockBasedStack
TEST(LockBasedStackTests, SingleThreadedPushPop) {
    LockBasedStack<int> s;
    const int N = 1000;
    for (int i = 0; i < N; ++i)
        s.push(i);

    std::vector<bool> seen(N, false);
    int cnt = 0;
    while (true) {
        auto v = s.pop();
        if (!v)
            break;
        int x = *v;
        ASSERT_GE(x, 0);
        ASSERT_LT(x, N);
        seen[x] = true;
        ++cnt;
    }
    ASSERT_EQ(cnt, N);
    for (int i = 0; i < N; ++i)
        ASSERT_TRUE(seen[i]);
}

// Simple multi-threaded stress test for LockFreeStack
TEST(LockFreeStackTests, MultiThreadedProducerConsumer) {
    // moderate sizes to keep test time reasonable in CI
    run_producer_consumer_test<LockFreeStack<int>>(4, 4, 2500);
}

// Simple multi-threaded stress test for LockBasedStack
TEST(LockBasedStackTests, MultiThreadedProducerConsumer) {
    run_producer_consumer_test<LockBasedStack<int>>(4, 4, 2500);
}

// Extra: check that pop on empty returns nullopt immediately and does not crash
TEST(LockFreeStackTests, PopEmpty) {
    LockFreeStack<int> s;
    auto v = s.pop();
    ASSERT_FALSE(v.has_value());
}

TEST(LockBasedStackTests, PopEmpty) {
    LockBasedStack<int> s;
    auto v = s.pop();
    ASSERT_FALSE(v.has_value());
}
