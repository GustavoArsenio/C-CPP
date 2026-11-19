#include <assert.h>
#include <stddef.h>

#include "int_min_max.h"

int main(void) {
    int minimum = 99;
    int maximum = -99;

    assert(int_min_max_four(12, -3, 7, 5, &minimum, &maximum));
    assert(minimum == -3);
    assert(maximum == 12);

    assert(int_min_max_four(-4, -4, -4, -4, &minimum, &maximum));
    assert(minimum == -4);
    assert(maximum == -4);

    minimum = 99;
    maximum = -99;
    assert(!int_min_max_four(1, 2, 3, 4, NULL, &maximum));
    assert(maximum == -99);
    assert(!int_min_max_four(1, 2, 3, 4, &minimum, NULL));
    assert(minimum == 99);
    assert(!int_min_max_four(1, 2, 3, 4, NULL, NULL));

    minimum = 42;
    assert(!int_min_max_four(1, 2, 3, 4, &minimum, &minimum));
    assert(minimum == 42);
    return 0;
}
