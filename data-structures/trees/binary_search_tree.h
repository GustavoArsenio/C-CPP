#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdbool.h>

struct bst_node;

typedef void (*bst_visit_fn)(int key, void *context);

/* The caller owns the tree and releases it with bst_destroy. */
bool bst_insert(struct bst_node **root, int key);
void bst_inorder(const struct bst_node *root, bst_visit_fn visit,
                 void *context);
void bst_destroy(struct bst_node *root);

#endif
