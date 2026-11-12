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

static void print_list(const struct int_list *list) {
    struct print_context context = {true};
    int_list_visit(list, print_value, &context);
    putchar('\n');
}

int main(void) {
    size_t count;
    int target;
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
    if (scanf("%d", &target) != 1) {
        int_list_destroy(list);
        return 1;
    }
    struct int_list *suffix = int_list_split_after(list, target);
    if (suffix == NULL) {
        int_list_destroy(list);
        return 1;
    }
    print_list(list);
    print_list(suffix);
    int_list_destroy(list);
    int_list_destroy(suffix);
    return 0;
}
