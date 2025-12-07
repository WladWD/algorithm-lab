#include <gtest/gtest.h>
#include "data_structures/lock_free/hash_map/hash_map.h"
#include <string>

using namespace data_structures::lock_free::hash_map;

TEST(HashMap, InsertFindErase) {
    LockFreeHashMap<std::string, int> m(16);
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);

    EXPECT_TRUE(m.insert("one", 1));
    EXPECT_FALSE(m.insert("one", 11)); // duplicate
    EXPECT_TRUE(m.insert("two", 2));
    EXPECT_EQ(m.size(), 2u);

    auto v = m.find("one");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 1);

    EXPECT_TRUE(m.erase("one"));
    EXPECT_FALSE(m.erase("one")); // already deleted
    EXPECT_EQ(m.size(), 1u);

    EXPECT_EQ(m.find("one"), std::nullopt);
}