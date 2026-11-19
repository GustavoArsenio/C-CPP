#include <iostream>
#include <memory>

#include "data-structures/trees/binary_tree.hpp"

int main() {
    using learning::trees::BinaryTreeNode;
    using learning::trees::count_nodes;

    auto root = std::make_unique<BinaryTreeNode>(1);
    root->left = std::make_unique<BinaryTreeNode>(2);
    root->right = std::make_unique<BinaryTreeNode>(3);
    root->left->left = std::make_unique<BinaryTreeNode>(4);
    root->left->right = std::make_unique<BinaryTreeNode>(5);
    root->right->left = std::make_unique<BinaryTreeNode>(6);
    root->right->right = std::make_unique<BinaryTreeNode>(7);

    std::cout << count_nodes(root.get()) << '\n';
}
