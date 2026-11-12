#include <stdio.h>

#include "double_list.h"

static bool read_values(struct double_list *list, size_t count) {
    for (size_t index = 0; index < count; ++index) {
        double value;
        if (scanf("%lf", &value) != 1 || !double_list_append(list, value)) {
            return false;
        }
    }
    return true;
}

struct print_context {
    bool first;
};

static void print_value(double value, void *opaque_context) {
    struct print_context *context = opaque_context;
    printf("%s%.17g", context->first ? "" : " ", value);
    context->first = false;
}

static void print_list(const struct double_list *list) {
    struct print_context context = {true};
    double_list_visit(list, print_value, &context);
    putchar('\n');
}

int main(void) {
    size_t first_count;
    size_t second_count;
    if (scanf("%zu", &first_count) != 1) {
        return 1;
    }
    struct double_list *first = double_list_create();
    struct double_list *second = double_list_create();
    if (first == NULL || second == NULL || !read_values(first, first_count) ||
        scanf("%zu", &second_count) != 1 ||
        !read_values(second, second_count) ||
        !double_list_concat_move(first, second)) {
        fputs("Invalid input or unable to concatenate lists.\n", stderr);
        double_list_destroy(first);
        double_list_destroy(second);
        return 1;
    }
    print_list(first);
    double_list_destroy(first);
    double_list_destroy(second);
    return 0;
}
