#include <stdbool.h>
#include <stdio.h>

#include "data-structures/linear/int_list.h"

struct print_context {
    bool first;
};

static void print_value(int value, void *opaque_context) {
    struct print_context *context = opaque_context;
    printf("%s%d", context->first ? "" : " ", value);
    context->first = false;
}

int main(void) {
    size_t count;
    if (scanf("%zu", &count) != 1) {
        return 1;
    }
    struct int_list *list = int_list_create();
    if (list == NULL) {
        return 1;
    }
    for (size_t i = 0; i < count; ++i) {
        int value;
        if (scanf("%d", &value) != 1 || !int_list_append(list, value)) {
            int_list_destroy(list);
            return 1;
        }
    }
    int_list_reverse(list);
    struct print_context context = {true};
    int_list_visit(list, print_value, &context);
    putchar('\n');
    int_list_destroy(list);
    return 0;
}
