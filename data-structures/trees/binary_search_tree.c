#include "binary_search_tree.h"

#include <stdlib.h>

struct bst_node {
    int key;
    struct bst_node *left;
    struct bst_node *right;
};

static struct bst_node *new_node(int key) {
    struct bst_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

bool bst_insert(struct bst_node **root, int key) {
    if (root == NULL) {
        return false;
    }
    if (*root == NULL) {
        *root = new_node(key);
        return *root != NULL;
    }
    if (key < (*root)->key) {
        return bst_insert(&(*root)->left, key);
    }
    return bst_insert(&(*root)->right, key);
}

void bst_inorder(const struct bst_node *root, bst_visit_fn visit,
                 void *context) {
    if (root == NULL || visit == NULL) {
        return;
    }
    bst_inorder(root->left, visit, context);
    visit(root->key, context);
    bst_inorder(root->right, visit, context);
}

void bst_destroy(struct bst_node *root) {
    if (root == NULL) {
        return;
    }
    bst_destroy(root->left);
    bst_destroy(root->right);
    free(root);
}
