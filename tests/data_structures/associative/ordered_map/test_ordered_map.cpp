#include <data_structures/associative/ordered_map/ordered_map.h>

#include <gtest/gtest.h>
#include <map>
#include <random>
#include <string>

namespace om = ds::ordered_map;
using Map    = om::OrderedMap<int, std::string>;

// ==================== basic operations ====================

TEST(OrderedMap, EmptyOnConstruct) {
    Map m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
}

TEST(OrderedMap, InsertAndFind) {
    Map m;
    m.insert(1, "one");
    ASSERT_NE(m.find(1), nullptr);
    EXPECT_EQ(*m.find(1), "one");
    EXPECT_EQ(m.size(), 1u);
}

TEST(OrderedMap, InsertMultiple) {
    Map m;
    m.insert(3, "three");
    m.insert(1, "one");
    m.insert(2, "two");
    EXPECT_EQ(m.size(), 3u);
    EXPECT_EQ(*m.find(1), "one");
    EXPECT_EQ(*m.find(2), "two");
    EXPECT_EQ(*m.find(3), "three");
}

TEST(OrderedMap, InsertOverwritesExisting) {
    Map m;
    m.insert(42, "old");
    m.insert(42, "new");
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(*m.find(42), "new");
}

TEST(OrderedMap, FindMissing) {
    Map m;
    m.insert(1, "one");
    EXPECT_EQ(m.find(99), nullptr);
}

TEST(OrderedMap, Contains) {
    Map m;
    m.insert(5, "five");
    EXPECT_TRUE(m.contains(5));
    EXPECT_FALSE(m.contains(6));
}

// ==================== erase ====================

TEST(OrderedMap, EraseLeaf) {
    Map m;
    m.insert(1, "one");
    EXPECT_TRUE(m.erase(1));
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.find(1), nullptr);
}

TEST(OrderedMap, EraseAbsent) {
    Map m;
    EXPECT_FALSE(m.erase(99));
}

TEST(OrderedMap, EraseRoot) {
    Map m;
    m.insert(2, "two");
    m.insert(1, "one");
    m.insert(3, "three");
    m.erase(2);
    EXPECT_EQ(m.size(), 2u);
    EXPECT_EQ(m.find(2), nullptr);
    EXPECT_NE(m.find(1), nullptr);
    EXPECT_NE(m.find(3), nullptr);
}

TEST(OrderedMap, EraseAndReinsert) {
    Map m;
    m.insert(1, "one");
    m.erase(1);
    m.insert(1, "ONE");
    EXPECT_EQ(*m.find(1), "ONE");
    EXPECT_EQ(m.size(), 1u);
}

TEST(OrderedMap, EraseNodeWithTwoChildren) {
    Map m;
    for (int k : {5, 3, 7, 2, 4, 6, 8}) m.insert(k, "v");
    m.erase(3);
    EXPECT_EQ(m.size(), 6u);
    EXPECT_EQ(m.find(3), nullptr);
    for (int k : {2, 4, 5, 6, 7, 8}) EXPECT_TRUE(m.contains(k));
}

// ==================== operator[] ====================

TEST(OrderedMap, SubscriptInserts) {
    Map m;
    m[7] = "seven";
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(*m.find(7), "seven");
}

TEST(OrderedMap, SubscriptReturnsRef) {
    Map m;
    m.insert(1, "a");
    m[1] = "b";
    EXPECT_EQ(*m.find(1), "b");
}

// ==================== min/max ====================

TEST(OrderedMap, MinMaxEmpty) {
    Map m;
    EXPECT_EQ(m.min_key(), nullptr);
    EXPECT_EQ(m.max_key(), nullptr);
}

TEST(OrderedMap, MinMaxSingle) {
    Map m;
    m.insert(42, "x");
    ASSERT_NE(m.min_key(), nullptr);
    ASSERT_NE(m.max_key(), nullptr);
    EXPECT_EQ(*m.min_key(), 42);
    EXPECT_EQ(*m.max_key(), 42);
}

TEST(OrderedMap, MinMaxMultiple) {
    Map m;
    for (int k : {5, 1, 8, 3, 9, 2}) m.insert(k, "v");
    EXPECT_EQ(*m.min_key(), 1);
    EXPECT_EQ(*m.max_key(), 9);
}

TEST(OrderedMap, MinUpdatesAfterErase) {
    Map m;
    m.insert(1, "one");
    m.insert(2, "two");
    m.erase(1);
    EXPECT_EQ(*m.min_key(), 2);
}

// ==================== clear ====================

TEST(OrderedMap, Clear) {
    Map m;
    m.insert(1, "one");
    m.insert(2, "two");
    m.clear();
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.find(1), nullptr);
}

TEST(OrderedMap, InsertAfterClear) {
    Map m;
    m.insert(1, "one");
    m.clear();
    m.insert(2, "two");
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(*m.find(2), "two");
}

// ==================== copy / move ====================

TEST(OrderedMap, CopyConstructor) {
    Map m;
    m.insert(1, "one");
    m.insert(2, "two");
    Map copy(m);
    EXPECT_EQ(copy.size(), 2u);
    EXPECT_EQ(*copy.find(1), "one");
    copy.insert(3, "three");
    EXPECT_EQ(m.size(), 2u); // original unaffected
}

TEST(OrderedMap, CopyAssignment) {
    Map m;
    m.insert(1, "one");
    Map m2;
    m2 = m;
    EXPECT_EQ(*m2.find(1), "one");
}

TEST(OrderedMap, MoveConstructor) {
    Map m;
    m.insert(1, "one");
    Map m2(std::move(m));
    EXPECT_EQ(m2.size(), 1u);
    EXPECT_EQ(*m2.find(1), "one");
    EXPECT_TRUE(m.empty());
}

// ==================== AVL balance (stress) ====================

TEST(OrderedMap, AscendingInsertsRemainBalanced) {
    // Ascending inserts are the worst case for a naive BST (becomes a list),
    // but AVL must stay O(log n).
    Map m;
    for (int i = 0; i < 1000; ++i) m.insert(i, "v");
    EXPECT_EQ(m.size(), 1000u);
    EXPECT_EQ(*m.min_key(), 0);
    EXPECT_EQ(*m.max_key(), 999);
    for (int i = 0; i < 1000; ++i) EXPECT_TRUE(m.contains(i));
}

TEST(OrderedMap, AgainstStdMap) {
    // Random operations against std::map as reference.
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> key_dist(0, 199);
    std::uniform_int_distribution<int> op_dist(0, 2);

    Map         our;
    std::map<int, std::string> ref;

    for (int round = 0; round < 2000; ++round) {
        int k = key_dist(rng);
        int op = op_dist(rng);
        if (op == 0) {
            std::string v = std::to_string(k);
            our.insert(k, v);
            ref[k] = v;
        } else if (op == 1) {
            bool ours = our.erase(k);
            bool refs = (ref.erase(k) > 0);
            EXPECT_EQ(ours, refs);
        } else {
            bool ours = our.contains(k);
            bool refs = ref.count(k) > 0;
            EXPECT_EQ(ours, refs);
        }
    }
    EXPECT_EQ(our.size(), ref.size());
}
