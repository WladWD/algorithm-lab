#include <data_structures/associative/hash_table_chaining/hash_table_chaining.h>

#include <gtest/gtest.h>
#include <string>

namespace ht = ds::hash_table_chaining;
using Table  = ht::HashTableChaining<int, std::string>;

// ==================== basic operations ====================

TEST(HashTableChaining, EmptyOnConstruct) {
    Table t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0u);
}

TEST(HashTableChaining, InsertAndFind) {
    Table t;
    t.insert(1, "one");
    ASSERT_NE(t.find(1), nullptr);
    EXPECT_EQ(*t.find(1), "one");
    EXPECT_EQ(t.size(), 1u);
}

TEST(HashTableChaining, InsertMultiple) {
    Table t;
    t.insert(1, "one");
    t.insert(2, "two");
    t.insert(3, "three");
    EXPECT_EQ(t.size(), 3u);
    EXPECT_EQ(*t.find(1), "one");
    EXPECT_EQ(*t.find(2), "two");
    EXPECT_EQ(*t.find(3), "three");
}

TEST(HashTableChaining, InsertOverwritesExisting) {
    Table t;
    t.insert(42, "old");
    t.insert(42, "new");
    EXPECT_EQ(t.size(), 1u);
    EXPECT_EQ(*t.find(42), "new");
}

TEST(HashTableChaining, FindMissing) {
    Table t;
    t.insert(1, "one");
    EXPECT_EQ(t.find(99), nullptr);
}

TEST(HashTableChaining, Contains) {
    Table t;
    t.insert(5, "five");
    EXPECT_TRUE(t.contains(5));
    EXPECT_FALSE(t.contains(6));
}

// ==================== erase ====================

TEST(HashTableChaining, ErasePresent) {
    Table t;
    t.insert(1, "one");
    EXPECT_TRUE(t.erase(1));
    EXPECT_EQ(t.size(), 0u);
    EXPECT_EQ(t.find(1), nullptr);
}

TEST(HashTableChaining, EraseAbsent) {
    Table t;
    EXPECT_FALSE(t.erase(99));
}

TEST(HashTableChaining, EraseAndReinsert) {
    Table t;
    t.insert(1, "one");
    t.erase(1);
    t.insert(1, "ONE");
    EXPECT_EQ(*t.find(1), "ONE");
    EXPECT_EQ(t.size(), 1u);
}

TEST(HashTableChaining, EraseFromChain) {
    // Force collisions by using a capacity-1 table (all keys in one bucket).
    ht::HashTableChaining<int, int> t(1);
    t.insert(0, 10);
    t.insert(1, 20);
    t.insert(2, 30);
    t.erase(1);
    EXPECT_EQ(t.find(1), nullptr);
    EXPECT_EQ(*t.find(0), 10);
    EXPECT_EQ(*t.find(2), 30);
}

// ==================== operator[] ====================

TEST(HashTableChaining, SubscriptInserts) {
    Table t;
    t[7] = "seven";
    EXPECT_EQ(t.size(), 1u);
    EXPECT_EQ(*t.find(7), "seven");
}

TEST(HashTableChaining, SubscriptReturnsRef) {
    Table t;
    t.insert(1, "a");
    t[1] = "b";
    EXPECT_EQ(*t.find(1), "b");
}

// ==================== clear ====================

TEST(HashTableChaining, Clear) {
    Table t;
    t.insert(1, "one");
    t.insert(2, "two");
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.find(1), nullptr);
}

// ==================== rehash / stress ====================

TEST(HashTableChaining, ManyInserts) {
    Table t;
    for (int i = 0; i < 1000; ++i)
        t.insert(i, std::to_string(i));
    EXPECT_EQ(t.size(), 1000u);
    for (int i = 0; i < 1000; ++i) {
        ASSERT_NE(t.find(i), nullptr) << "key=" << i;
        EXPECT_EQ(*t.find(i), std::to_string(i));
    }
}

TEST(HashTableChaining, ManyInsertsAndErases) {
    Table t;
    for (int i = 0; i < 500; ++i) t.insert(i, "x");
    for (int i = 0; i < 250; ++i) t.erase(i);
    EXPECT_EQ(t.size(), 250u);
    for (int i = 250; i < 500; ++i) EXPECT_TRUE(t.contains(i));
    for (int i = 0;   i < 250; ++i) EXPECT_FALSE(t.contains(i));
}

TEST(HashTableChaining, LoadFactorBounded) {
    Table t;
    for (int i = 0; i < 200; ++i) t.insert(i, "v");
    EXPECT_LE(t.load_factor(), 1.0 + 1e-9);
}

TEST(HashTableChaining, StringKeys) {
    ht::HashTableChaining<std::string, int> t;
    t.insert("alpha", 1);
    t.insert("beta",  2);
    t.insert("gamma", 3);
    EXPECT_EQ(*t.find("alpha"), 1);
    EXPECT_EQ(*t.find("beta"),  2);
    EXPECT_EQ(t.find("delta"), nullptr);
}
