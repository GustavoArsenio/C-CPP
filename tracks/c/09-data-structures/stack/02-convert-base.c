#include <stdio.h>

#include "int_stack.h"

static char digit_symbol(int value) {
    return value < 10 ? (char)('0' + value) : (char)('A' + value - 10);
}

int main(void) {
    long long number;
    int base;
    if (scanf("%lld %d", &number, &base) != 2 || number < 0 || base < 2 ||
        base > 36) {
        fputs("Expected a non-negative integer and a base from 2 to 36.\n",
              stderr);
        return 1;
    }

    struct int_stack *digits = int_stack_create();
    if (digits == NULL) {
        fputs("Unable to create digit stack.\n", stderr);
        return 1;
    }

    do {
        const int remainder = (int)(number % base);
        if (!int_stack_push(digits, remainder)) {
            fputs("Unable to store converted digit.\n", stderr);
            int_stack_destroy(digits);
            return 1;
        }
        number /= base;
    } while (number > 0);

    int digit;
    while (int_stack_pop(digits, &digit)) {
        putchar(digit_symbol(digit));
    }
    putchar('\n');
    int_stack_destroy(digits);
    return 0;
}
