/* Allocate, initialize, sum, and release a dynamic array in C. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const size_t count = 5;
    int *values = malloc(count * sizeof *values);
    if (values == NULL) {
        fputs("error: unable to allocate integer array\n", stderr);
        return 1;
    }

    for (size_t index = 0; index < count; ++index) {
        values[index] = (int)index + 1;
    }

    int sum = 0;
    for (size_t index = 0; index < count; ++index) {
        sum += values[index];
    }
    printf("Sum = %d\n", sum);
    free(values);
    return 0;
}
