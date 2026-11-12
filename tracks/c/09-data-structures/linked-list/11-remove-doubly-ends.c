#include <stdio.h>

#include "lista4_lists.h"

static void print_int(int value, void *context) {
    bool *first = context;
    printf("%s%d", *first ? "" : " ", value);
    *first = false;
}

int main(void) {
    size_t count;
    if (scanf("%zu", &count) != 1) {
        return 1;
    }
    struct doubly_int_list *list = doubly_int_list_create();
    if (list == NULL) {
        return 1;
    }
    for (size_t i = 0; i < count; ++i) {
        int value;
        if (scanf("%d", &value) != 1 || !doubly_int_list_append(list, value)) {
            doubly_int_list_destroy(list);
            return 1;
        }
    }
    int first_value;
    int last_value;
    bool removed_first = doubly_int_list_remove_first(list, &first_value);
    bool removed_last = doubly_int_list_remove_last(list, &last_value);
    if (removed_first) {
        printf("first %d\n", first_value);
    }
    if (removed_last) {
        printf("last %d\n", last_value);
    }
    bool first = true;
    doubly_int_list_visit(list, print_int, &first);
    putchar('\n');
    doubly_int_list_destroy(list);
    return 0;
}
