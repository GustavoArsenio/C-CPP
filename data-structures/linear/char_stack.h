#ifndef C_CPP_CHAR_STACK_H
#define C_CPP_CHAR_STACK_H

#include <stdbool.h>
#include <stddef.h>

struct char_stack;

struct char_stack *char_stack_create(void);
bool char_stack_push(struct char_stack *stack, char value);
bool char_stack_pop(struct char_stack *stack, char *value);
bool char_stack_peek(const struct char_stack *stack, char *value);
size_t char_stack_size(const struct char_stack *stack);
void char_stack_destroy(struct char_stack *stack);

#endif
