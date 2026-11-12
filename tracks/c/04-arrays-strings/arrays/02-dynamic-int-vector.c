#include <stdint.h>
#include <stdio.h>

#include "int_vector.h"

int main(void) {
    long long requested_count = 0;
    if (scanf("%lld", &requested_count) != 1 || requested_count < 0 ||
        (uintmax_t)requested_count > SIZE_MAX) {
        fputs("error: expected an element count\n", stderr);
        return 1;
    }
    const size_t count = (size_t)requested_count;

    int_vector *values = int_vector_create();
    if (values == NULL) {
        fputs("error: unable to create integer vector\n", stderr);
        return 1;
    }

    for (size_t index = 0; index < count; ++index) {
        int value = 0;
        if (scanf("%d", &value) != 1) {
            fputs("error: expected an integer value\n", stderr);
            int_vector_destroy(values);
            return 1;
        }
        if (!int_vector_push_back(values, value)) {
            fputs("error: unable to grow integer vector\n", stderr);
            int_vector_destroy(values);
            return 1;
        }
    }

    printf("Count: %zu, values:", int_vector_size(values));
    for (size_t index = 0; index < int_vector_size(values); ++index) {
        int value = 0;
        if (!int_vector_get(values, index, &value)) {
            fputs("error: invalid vector index\n", stderr);
            int_vector_destroy(values);
            return 1;
        }
        printf(" %d", value);
    }
    putchar('\n');
    int_vector_destroy(values);
    return 0;
}
