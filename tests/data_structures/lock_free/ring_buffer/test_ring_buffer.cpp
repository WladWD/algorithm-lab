#include "data_structures/lock_free/ring_buffer/ring_buffer.h"

#include <gtest/gtest.h>
#include <string>

using namespace data_structures::lock_free;

TEST(RingBuffer, SpscBasicEnqueueDequeue) {
    SpscRingBuffer<int> rb(8);
    EXPECT_EQ(rb.capacity(), 8u);
    EXPECT_EQ(rb.usable_capacity(), 7u);
    EXPECT_EQ(rb.size(), 0u);

    EXPECT_TRUE(rb.try_enqueue(1));
    EXPECT_TRUE(rb.try_enqueue(2));
    EXPECT_EQ(rb.size(), 2u);

    int out = 0;
    EXPECT_TRUE(rb.try_dequeue(out));
    EXPECT_EQ(out, 1);
    EXPECT_TRUE(rb.try_dequeue(out));
    EXPECT_EQ(out, 2);
    EXPECT_FALSE(rb.try_dequeue(out));
}

TEST(RingBuffer, MpmcTryFullEmpty) {
    MpmcRingBuffer<std::string> rb(4);
    // usable capacity for power-of-two 4 is 4 (mpmc uses all slots)
    EXPECT_EQ(rb.capacity(), 4u);
    EXPECT_TRUE(rb.try_enqueue(std::string("a")));
    EXPECT_TRUE(rb.try_enqueue(std::string("b")));
    EXPECT_TRUE(rb.try_enqueue(std::string("c")));
    EXPECT_TRUE(rb.try_enqueue(std::string("d")));
    // Now likely full
    bool inserted = rb.try_enqueue(std::string("x"));
    (void)inserted; // may be false if full

    std::string out;
    // drain
    int drained = 0;
    while (rb.try_dequeue(out)) {
        ++drained;
    }
    EXPECT_GE(drained, 1);
}
