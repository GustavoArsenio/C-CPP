#include <stdio.h>

#include "int_min_max.h"

int main(void) {
    int minimum;
    int maximum;

    if (!int_min_max_four(12, -3, 7, 5, &minimum, &maximum)) {
        return 1;
    }

    printf("Minimum: %d\nMaximum: %d\n", minimum, maximum);
    return 0;
}
