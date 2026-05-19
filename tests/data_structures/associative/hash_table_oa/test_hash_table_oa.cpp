#include <data_structures/associative/hash_table_oa/hash_table_oa.h>

#include <gtest/gtest.h>
#include <string>

namespace ht = ds::hash_table_oa;
using Table  = ht::HashTableOA<int, std::string>;

// ==================== basic operations ====================

TEST(HashTableOA, EmptyOnConstruct) {
    Table t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0u);
}

TEST(HashTableOA, InsertAndFind) {
    Table t;
    t.insert(1, "one");
    ASSERT_NE(t.find(1), nullptr);
    EXPECT_EQ(*t.find(1), "one");
    EXPECT_EQ(t.size(), 1u);
}

TEST(HashTableOA, InsertMultiple) {
    Table t;
    t.insert(1, "one");
    t.insert(2, "two");
    t.insert(3, "three");
    EXPECT_EQ(t.size(), 3u);
    EXPECT_EQ(*t.find(1), "one");
    EXPECT_EQ(*t.find(2), "two");
    EXPECT_EQ(*t.find(3), "three");
}

TEST(HashTableOA, InsertOverwritesExisting) {
    Table t;
    t.insert(42, "old");
    t.insert(42, "new");
    EXPECT_EQ(t.size(), 1u);
    EXPECT_EQ(*t.find(42), "new");
}

TEST(HashTableOA, FindMissing) {
    Table t;
    t.insert(1, "one");
    EXPECT_EQ(t.find(99), nullptr);
}

TEST(HashTableOA, Contains) {
    Table t;
    t.insert(5, "five");
    EXPECT_TRUE(t.contains(5));
    EXPECT_FALSE(t.contains(6));
}

// ==================== erase ====================

TEST(HashTableOA, ErasePresent) {
    Table t;
    t.insert(1, "one");
    EXPECT_TRUE(t.erase(1));
    EXPECT_EQ(t.size(), 0u);
    EXPECT_EQ(t.find(1), nullptr);
}

TEST(HashTableOA, EraseAbsent) {
    Table t;
    EXPECT_FALSE(t.erase(99));
}

TEST(HashTableOA, EraseAndReinsert) {
    Table t;
    t.insert(1, "one");
    t.erase(1);
    t.insert(1, "ONE");
    EXPECT_EQ(*t.find(1), "ONE");
    EXPECT_EQ(t.size(), 1u);
}

TEST(HashTableOA, TombstoneDoesNotBlockLater) {
    // Insert keys that may share bucket via modular collision,
    // erase one and verify the other is still findable.
    ht::HashTableOA<int, int> t(4);
    t.insert(0, 100);
    t.insert(4, 200); // hashes to same bucket as 0 (mod 4 == 0) after rehash
    t.erase(0);
    EXPECT_EQ(*t.find(4), 200);
    EXPECT_EQ(t.find(0), nullptr);
}

// ==================== operator[] ====================

TEST(HashTableOA, SubscriptInserts) {
    Table t;
    t[7] = "seven";
    EXPECT_EQ(t.size(), 1u);
    EXPECT_EQ(*t.find(7), "seven");
}

TEST(HashTableOA, SubscriptReturnsRef) {
    Table t;
    t.insert(1, "a");
    t[1] = "b";
    EXPECT_EQ(*t.find(1), "b");
}

// ==================== clear ====================

TEST(HashTableOA, Clear) {
    Table t;
    t.insert(1, "one");
    t.insert(2, "two");
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.find(1), nullptr);
}

// ==================== rehash / stress ====================

TEST(HashTableOA, ManyInserts) {
    Table t;
    for (int i = 0; i < 1000; ++i)
        t.insert(i, std::to_string(i));
    EXPECT_EQ(t.size(), 1000u);
    for (int i = 0; i < 1000; ++i) {
        ASSERT_NE(t.find(i), nullptr) << "key=" << i;
        EXPECT_EQ(*t.find(i), std::to_string(i));
    }
}

TEST(HashTableOA, ManyInsertsAndErases) {
    Table t;
    for (int i = 0; i < 500; ++i) t.insert(i, "x");
    for (int i = 0; i < 250; ++i) t.erase(i);
    EXPECT_EQ(t.size(), 250u);
    for (int i = 250; i < 500; ++i) EXPECT_TRUE(t.contains(i));
    for (int i = 0; i < 250; ++i)   EXPECT_FALSE(t.contains(i));
}

TEST(HashTableOA, LoadFactorBounded) {
    Table t;
    for (int i = 0; i < 200; ++i) t.insert(i, "v");
    EXPECT_LE(t.load_factor(), 0.75 + 1e-9);
}
