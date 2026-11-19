#include <stdio.h>

#include "lista4_lists.h"

static void print_int(int value, void *context) {
    bool *first = context;
    printf("%s%d", *first ? "" : " ", value);
    *first = false;
}

int main(void) {
    size_t operations;
    if (scanf("%zu", &operations) != 1) {
        return 1;
    }
    struct circular_doubly_int_list *list = circular_doubly_int_list_create();
    if (list == NULL) {
        return 1;
    }
    for (size_t i = 0; i < operations; ++i) {
        char command;
        size_t index;
        if (scanf(" %c %zu", &command, &index) != 2) {
            circular_doubly_int_list_destroy(list);
            return 1;
        }
        if (command == 'I') {
            int value;
            if (scanf("%d", &value) != 1 ||
                !circular_doubly_int_list_insert(list, index, value)) {
                circular_doubly_int_list_destroy(list);
                return 1;
            }
        } else if (command == 'R') {
            int value;
            if (!circular_doubly_int_list_remove(list, index, &value)) {
                circular_doubly_int_list_destroy(list);
                return 1;
            }
            printf("removed %d\n", value);
        } else {
            circular_doubly_int_list_destroy(list);
            return 1;
        }
    }
    bool first = true;
    circular_doubly_int_list_visit(list, print_int, &first);
    putchar('\n');
    circular_doubly_int_list_destroy(list);
    return 0;
}
