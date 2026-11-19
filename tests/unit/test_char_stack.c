#include <assert.h>

#include "char_stack.h"

int main(void) {
    assert(char_stack_size(NULL) == 0);
    assert(!char_stack_push(NULL, 'x'));
    assert(!char_stack_pop(NULL, NULL));

    struct char_stack *stack = char_stack_create();
    assert(stack != NULL);
    char value = '?';
    assert(char_stack_size(stack) == 0);
    assert(!char_stack_peek(stack, &value));
    assert(!char_stack_pop(stack, &value));
    assert(!char_stack_pop(stack, NULL));

    assert(char_stack_push(stack, 'a'));
    assert(char_stack_push(stack, ' '));
    assert(char_stack_push(stack, 'Z'));
    assert(char_stack_size(stack) == 3);
    assert(char_stack_peek(stack, &value));
    assert(value == 'Z');
    assert(char_stack_pop(stack, &value) && value == 'Z');
    assert(char_stack_pop(stack, &value) && value == ' ');
    assert(char_stack_pop(stack, &value) && value == 'a');
    assert(char_stack_size(stack) == 0);
    assert(!char_stack_pop(stack, &value));

    char_stack_destroy(stack);
    char_stack_destroy(NULL);
    return 0;
}
