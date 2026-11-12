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
    struct circular_int_list *list = circular_int_list_create();
    if (list == NULL) {
        return 1;
    }
    for (size_t i = 0; i < count; ++i) {
        int value;
        if (scanf("%d", &value) != 1 ||
            !circular_int_list_append(list, value)) {
            circular_int_list_destroy(list);
            return 1;
        }
    }
    if (!circular_int_list_make_circular(list)) {
        circular_int_list_destroy(list);
        return 1;
    }
    bool first = true;
    circular_int_list_visit(list, print_int, &first);
    putchar('\n');
    circular_int_list_destroy(list);
    return 0;
}
