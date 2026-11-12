/* Binary search tree insertion, inorder traversal, and cleanup. */

#include "binary_search_tree.h"

#include <stdio.h>

static void print_key(int key, void *context) {
    (void)context;
    printf("%d ", key);
}

int main(void) {
    const int values[] = {5, 3, 7, 2, 4, 6, 8};
    struct bst_node *root = NULL;
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        if (!bst_insert(&root, values[i])) {
            fprintf(stderr, "Unable to allocate tree node\n");
            bst_destroy(root);
            return 1;
        }
    }
    printf("Inorder: ");
    bst_inorder(root, print_key, NULL);
    putchar('\n');
    bst_destroy(root);
    return 0;
}
