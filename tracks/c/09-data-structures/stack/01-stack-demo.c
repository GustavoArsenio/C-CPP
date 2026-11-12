#include <stdio.h>

#include "int_stack.h"

int main(void) {
    struct int_stack *stack = int_stack_create();
    if (stack == NULL) {
        fputs("Unable to create stack.\n", stderr);
        return 1;
    }

    const int values[] = {3, 1, 4};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        if (!int_stack_push(stack, values[i])) {
            fputs("Unable to push stack value.\n", stderr);
            int_stack_destroy(stack);
            return 1;
        }
    }

    int value;
    bool first = true;
    while (int_stack_pop(stack, &value)) {
        printf("%s%d", first ? "" : " ", value);
        first = false;
    }
    putchar('\n');
    int_stack_destroy(stack);
    return 0;
}
