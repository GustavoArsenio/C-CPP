#ifndef C_CPP_INT_STACK_H
#define C_CPP_INT_STACK_H

#include <stdbool.h>
#include <stddef.h>

struct int_stack;
struct int_list;

struct int_stack *int_stack_create(void);
bool int_stack_push(struct int_stack *stack, int value);
bool int_stack_pop(struct int_stack *stack, int *value);
bool int_stack_peek(const struct int_stack *stack, int *value);
size_t int_stack_size(const struct int_stack *stack);
void int_stack_destroy(struct int_stack *stack);

#endif
