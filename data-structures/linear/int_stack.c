#include "int_stack.h"

#include <stdlib.h>

struct stack_node {
    int value;
    struct stack_node *next;
};

struct int_stack {
    struct stack_node *top;
    size_t size;
};

struct int_stack *int_stack_create(void) {
    return calloc(1, sizeof(struct int_stack));
}

bool int_stack_push(struct int_stack *stack, int value) {
    if (stack == NULL) {
        return false;
    }

    struct stack_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }

    node->value = value;
    node->next = stack->top;
    stack->top = node;
    ++stack->size;
    return true;
}

bool int_stack_pop(struct int_stack *stack, int *value) {
    if (stack == NULL || value == NULL || stack->top == NULL) {
        return false;
    }

    struct stack_node *node = stack->top;
    *value = node->value;
    stack->top = node->next;
    --stack->size;
    free(node);
    return true;
}

bool int_stack_peek(const struct int_stack *stack, int *value) {
    if (stack == NULL || value == NULL || stack->top == NULL) {
        return false;
    }

    *value = stack->top->value;
    return true;
}

size_t int_stack_size(const struct int_stack *stack) {
    return stack == NULL ? 0 : stack->size;
}

void int_stack_destroy(struct int_stack *stack) {
    if (stack == NULL) {
        return;
    }

    struct stack_node *node = stack->top;
    while (node != NULL) {
        struct stack_node *next = node->next;
        free(node);
        node = next;
    }
    free(stack);
}
