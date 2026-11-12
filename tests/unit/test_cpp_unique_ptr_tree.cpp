#include <cstdlib>
#include <memory>
#include <type_traits>
#include <utility>

#include "data-structures/trees/binary_tree.hpp"

using learning::trees::BinaryTreeNode;
using learning::trees::count_nodes;

static_assert(!std::is_copy_constructible_v<BinaryTreeNode>);
static_assert(std::is_move_constructible_v<BinaryTreeNode>);

namespace {

void check(bool condition) {
    if (!condition) {
        std::abort();
    }
}

std::unique_ptr<BinaryTreeNode> make_node(int value) {
    return std::make_unique<BinaryTreeNode>(value);
}

}  // namespace

int main() {
    check(count_nodes(nullptr) == 0);

    auto leaf = make_node(1);
    check(count_nodes(leaf.get()) == 1);

    auto left_only = make_node(1);
    left_only->left = make_node(2);
    check(count_nodes(left_only.get()) == 2);

    auto right_only = make_node(1);
    right_only->right = make_node(2);
    check(count_nodes(right_only.get()) == 2);

    auto balanced = make_node(1);
    balanced->left = make_node(2);
    balanced->right = make_node(3);
    balanced->left->left = make_node(4);
    balanced->left->right = make_node(5);
    balanced->right->left = make_node(6);
    balanced->right->right = make_node(7);
    check(count_nodes(balanced.get()) == 7);

    auto irregular = make_node(1);
    irregular->left = make_node(2);
    irregular->left->right = make_node(3);
    irregular->left->right->left = make_node(4);
    irregular->right = make_node(5);
    check(count_nodes(irregular.get()) == 5);

    auto moved = std::move(balanced);
    check(balanced == nullptr);
    check(count_nodes(moved.get()) == 7);
}
