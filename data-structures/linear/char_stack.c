#include "char_stack.h"

#include <stdlib.h>

struct char_stack_node {
    char value;
    struct char_stack_node *next;
};

struct char_stack {
    struct char_stack_node *top;
    size_t size;
};

struct char_stack *char_stack_create(void) {
    return calloc(1, sizeof(struct char_stack));
}

bool char_stack_push(struct char_stack *stack, char value) {
    if (stack == NULL) {
        return false;
    }
    struct char_stack_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }
    node->value = value;
    node->next = stack->top;
    stack->top = node;
    ++stack->size;
    return true;
}

bool char_stack_pop(struct char_stack *stack, char *value) {
    if (stack == NULL || value == NULL || stack->top == NULL) {
        return false;
    }
    struct char_stack_node *node = stack->top;
    *value = node->value;
    stack->top = node->next;
    --stack->size;
    free(node);
    return true;
}

bool char_stack_peek(const struct char_stack *stack, char *value) {
    if (stack == NULL || value == NULL || stack->top == NULL) {
        return false;
    }
    *value = stack->top->value;
    return true;
}

size_t char_stack_size(const struct char_stack *stack) {
    return stack == NULL ? 0 : stack->size;
}

void char_stack_destroy(struct char_stack *stack) {
    if (stack == NULL) {
        return;
    }
    struct char_stack_node *node = stack->top;
    while (node != NULL) {
        struct char_stack_node *next = node->next;
        free(node);
        node = next;
    }
    free(stack);
}
