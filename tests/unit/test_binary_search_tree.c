#include "binary_search_tree.h"

#include <assert.h>
#include <stddef.h>

struct keys {
    int values[8];
    size_t count;
};

static void collect_key(int key, void *context) {
    struct keys *keys = context;
    assert(keys->count < sizeof(keys->values) / sizeof(keys->values[0]));
    keys->values[keys->count++] = key;
}

int main(void) {
    struct bst_node *root = NULL;
    struct keys keys = {{0}, 0};
    bst_inorder(NULL, collect_key, &keys);
    bst_inorder(NULL, NULL, NULL);
    assert(!bst_insert(NULL, 10));
    assert(bst_insert(&root, 5));
    assert(bst_insert(&root, 5));
    assert(bst_insert(&root, 3));
    assert(bst_insert(&root, 7));
    bst_inorder(root, collect_key, &keys);
    assert(keys.count == 4);
    assert(keys.values[0] == 3 && keys.values[1] == 5);
    assert(keys.values[2] == 5 && keys.values[3] == 7);
    bst_destroy(root);
    bst_destroy(NULL);
    return 0;
}
