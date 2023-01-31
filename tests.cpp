#include <iostream>
#include <gtest/gtest.h>
#include "avltree.h"

TEST(AvlTreeTest, InsertTest) {
    avltree<int, std::string> tree;
    tree.insert(1, "First");
    tree.insert(2, "Second");
    tree.insert(3, "Third");

    ASSERT_EQ(tree[1], "First");
    ASSERT_EQ(tree[2], "Second");
    ASSERT_EQ(tree[3], "Third");
}

TEST(AvlTreeTest, DeleteTest) {
    avltree<int, std::string> tree;
    tree.insert(1, "First");
    tree.insert(2, "Second");
    tree.insert(3, "Third");
    tree.deleteNode(1);
    tree.deleteNode(2);

    ASSERT_FALSE(tree.search(1));
    ASSERT_FALSE(tree.search(2));
    ASSERT_EQ(tree[3], "Third");
}


int main(int argc, char **argv) {
    avltree<int, std::string> tree;
    tree.insert(1, "One");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

