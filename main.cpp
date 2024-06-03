#include <gtest/gtest.h>
#include "HashTable.h"

TEST(HashTableTest, InsertAndRetrieveItem) {
    HashTable<int, std::string> hashTable(10);
    hashTable.insertItem(1, "one");
    ASSERT_EQ(hashTable.get(1), "one");
}

TEST(HashTableTest, DeleteItem) {
    HashTable<int, std::string> hashTable(10);
    hashTable.insertItem(1, "one");
    hashTable.deleteItem(1);
    ASSERT_EQ(hashTable.get(1), std::string());
}

TEST(HashTableTest, HandleCollision) {
    HashTable<int, std::string> hashTable(1); // Force a collision
    hashTable.insertItem(1, "one");
    hashTable.insertItem(2, "two");
    ASSERT_EQ(hashTable.get(1), "one");
    ASSERT_EQ(hashTable.get(2), "two");
}

TEST(HashTableTest, Rehash) {
    HashTable<int, std::string> hashTable(1); // Force a rehash
    for (int i = 0; i < 100; i++) {
        hashTable.insertItem(i, std::to_string(i));
    }
    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(hashTable.get(i), std::to_string(i));
    }
}

TEST(HashTableTest, RetrieveNonExistentItem) {
    HashTable<int, std::string> hashTable(10);
    ASSERT_EQ(hashTable.get(1), std::string());
}
TEST(HashTableTest, DeleteNonExistentItem) {
    HashTable<int, std::string> hashTable(10);
    hashTable.deleteItem(1);
    ASSERT_EQ(hashTable.get(1), std::string());
}
TEST(HashTableTest, InsertAndRetrieveItemWithStringKey) {
    HashTable<std::string, int> hashTable(10);
    hashTable.insertItem("one", 1);
    ASSERT_EQ(hashTable.get("one"), 1);
}

TEST(HashTableTest, DisplayHash) {
    HashTable<int, std::string> hashTable(10);
    hashTable.insertItem(1, "one");
    hashTable.insertItem(2, "two");
    hashTable.insertItem(3, "three");
    hashTable.insertItem(4, "four");
    hashTable.insertItem(5, "five");
    hashTable.displayHash();
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
