#include <stdbool.h>
#include <stdio.h>

#include "int_stack.h"

static bool move_top(struct int_stack *source, struct int_stack *destination) {
    int value;
    return int_stack_pop(source, &value) && int_stack_push(destination, value);
}

int main(void) {
    struct int_stack *first = int_stack_create();
    struct int_stack *second = int_stack_create();
    struct int_stack *third = int_stack_create();
    if (first == NULL || second == NULL || third == NULL) {
        fputs("Unable to create stacks.\n", stderr);
        int_stack_destroy(first);
        int_stack_destroy(second);
        int_stack_destroy(third);
        return 1;
    }

    const int initial_values[] = {1, 2, 3, 4};
    for (size_t i = 0; i < sizeof(initial_values) / sizeof(initial_values[0]);
         ++i) {
        if (!int_stack_push(first, initial_values[i])) {
            fputs("Unable to populate first stack.\n", stderr);
            int_stack_destroy(first);
            int_stack_destroy(second);
            int_stack_destroy(third);
            return 1;
        }
    }

    const bool transfers_succeeded =
        move_top(first, second) && move_top(first, third) &&
        move_top(first, second) && move_top(first, third) &&
        move_top(second, first) && move_top(second, third) &&
        move_top(first, third);
    if (!transfers_succeeded) {
        fputs("Unable to transfer stack value.\n", stderr);
        int_stack_destroy(first);
        int_stack_destroy(second);
        int_stack_destroy(third);
        return 1;
    }

    bool first_value = true;
    int value;
    while (int_stack_pop(third, &value)) {
        printf("%s%d", first_value ? "" : " ", value);
        first_value = false;
    }
    putchar('\n');

    int_stack_destroy(first);
    int_stack_destroy(second);
    int_stack_destroy(third);
    return 0;
}
