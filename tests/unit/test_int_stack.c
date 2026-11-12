#include <stdio.h>

#include "int_stack.h"

#define CHECK(condition)                               \
    do {                                               \
        if (!(condition)) {                            \
            fprintf(stderr, "FAIL: %s\n", #condition); \
            return 1;                                  \
        }                                              \
    } while (0)

int main(void) {
    struct int_stack *stack = int_stack_create();
    CHECK(stack != NULL);
    CHECK(int_stack_size(stack) == 0);

    int value = 0;
    CHECK(!int_stack_pop(stack, &value));
    CHECK(!int_stack_peek(stack, &value));
    CHECK(int_stack_push(stack, 10));
    CHECK(int_stack_push(stack, -4));
    CHECK(int_stack_size(stack) == 2);
    CHECK(int_stack_peek(stack, &value));
    CHECK(value == -4);
    CHECK(int_stack_pop(stack, &value));
    CHECK(value == -4);
    CHECK(int_stack_size(stack) == 1);
    CHECK(int_stack_pop(stack, &value));
    CHECK(value == 10);
    CHECK(int_stack_size(stack) == 0);
    CHECK(!int_stack_pop(stack, &value));

    int_stack_destroy(stack);
    int_stack_destroy(NULL);
    CHECK(!int_stack_push(NULL, 1));
    CHECK(!int_stack_pop(NULL, &value));
    CHECK(!int_stack_peek(NULL, &value));
    CHECK(int_stack_size(NULL) == 0);
    return 0;
}
