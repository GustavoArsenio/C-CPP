#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <cstddef>
#include <memory>

namespace learning::trees {

struct BinaryTreeNode {
    int value;
    std::unique_ptr<BinaryTreeNode> left;
    std::unique_ptr<BinaryTreeNode> right;

    explicit BinaryTreeNode(int node_value) noexcept : value(node_value) {}
};

// Counts a borrowed tree in O(n) time and O(h) recursive stack space.
// Counting and unique_ptr destruction use stack space proportional to height;
// extremely deep trees can exhaust the stack in either operation.
inline std::size_t count_nodes(const BinaryTreeNode* root) noexcept {
    if (root == nullptr) {
        return 0;
    }
    return 1 + count_nodes(root->left.get()) + count_nodes(root->right.get());
}

}  // namespace learning::trees

#endif
