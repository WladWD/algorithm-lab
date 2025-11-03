#include "data_structures/lock_free/queue/queue.h"

#include <atomic>
#include <gtest/gtest.h>
#include <numeric>
#include <thread>
#include <vector>

using namespace data_structures::lock_free::queue;

TEST(LockFreeQueueTest, SingleThreadedFIFO) {
    LockFreeQueue<int> q;
    const int N = 1000;
    for (int i = 0; i < N; ++i)
        q.push(i);
    for (int i = 0; i < N; ++i) {
        auto v = q.pop();
        ASSERT_TRUE(v.has_value());
        EXPECT_EQ(*v, i);
    }
    EXPECT_FALSE(q.pop().has_value());
}

TEST(LockFreeQueueTest, MultiProducerMultiConsumer) {
    LockFreeQueue<int> q;
    const int producers = 4;
    const int consumers = 4;
    const int per_producer = 2000;
    const int total = producers * per_producer;

    std::atomic<int> produced{0};
    std::atomic<int> consumed{0};
    std::atomic<long long> sum{0};

    std::vector<std::thread> prod;
    prod.reserve(producers);
    for (int p = 0; p < producers; ++p) {
        prod.emplace_back([&q, p, per_producer, &produced]() {
            int base = p * per_producer;
            for (int i = 0; i < per_producer; ++i) {
                q.push(base + i);
                produced.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    std::vector<std::thread> cons;
    cons.reserve(consumers);
    for (int c = 0; c < consumers; ++c) {
        cons.emplace_back([&q, &consumed, &sum, total]() {
            while (consumed.load(std::memory_order_relaxed) < total) {
                auto v = q.pop();
                if (v) {
                    sum.fetch_add(*v, std::memory_order_relaxed);
                    consumed.fetch_add(1, std::memory_order_relaxed);
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }

    for (auto& t : prod)
        t.join();
    for (auto& t : cons)
        t.join();

    EXPECT_EQ(produced.load(), total);
    EXPECT_EQ(consumed.load(), total);

    // expected sum is 0..total-1 but producers have blocks; compute expected
    long long expected = 0;
    for (int p = 0; p < producers; ++p) {
        int base = p * per_producer;
        for (int i = 0; i < per_producer; ++i)
            expected += base + i;
    }
    EXPECT_EQ(sum.load(), expected);
}

TEST(LockFreeQueueTest, EmptyPopReturnsNullopt) {
    LockFreeQueue<int> q;
    auto v = q.pop();
    EXPECT_FALSE(v.has_value());
}
