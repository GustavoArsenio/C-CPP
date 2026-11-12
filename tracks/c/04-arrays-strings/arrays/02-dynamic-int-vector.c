#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include "int_vector.h"

static int read_decimal(uintmax_t positive_limit, uintmax_t negative_limit,
                        int allow_sign, uintmax_t *magnitude,
                        int *is_negative) {
    int character = 0;
    do {
        character = fgetc(stdin);
    } while (character != EOF && isspace((unsigned char)character));

    int negative = 0;
    if (character == '-' || character == '+') {
        if (!allow_sign) {
            return 0;
        }
        negative = character == '-';
        character = fgetc(stdin);
    }
    if (character < '0' || character > '9') {
        return 0;
    }

    const uintmax_t limit = negative ? negative_limit : positive_limit;
    uintmax_t parsed = 0;
    do {
        const uintmax_t digit = (uintmax_t)(character - '0');
        if (parsed > (limit - digit) / 10) {
            return 0;
        }
        parsed = parsed * 10 + digit;
        character = fgetc(stdin);
    } while (character >= '0' && character <= '9');

    if (character != EOF && !isspace((unsigned char)character)) {
        return 0;
    }
    *magnitude = parsed;
    *is_negative = negative;
    return 1;
}

static int read_count(size_t *count) {
    uintmax_t magnitude = 0;
    int is_negative = 0;
    if (!read_decimal(SIZE_MAX, 0, 0, &magnitude, &is_negative)) {
        return 0;
    }
    *count = (size_t)magnitude;
    return 1;
}

static int read_integer(int *value) {
    uintmax_t magnitude = 0;
    int is_negative = 0;
    const uintmax_t negative_limit = (uintmax_t)(-(INT_MIN + 1)) + 1;
    if (!read_decimal((uintmax_t)INT_MAX, negative_limit, 1, &magnitude,
                      &is_negative)) {
        return 0;
    }
    if (is_negative && magnitude == negative_limit) {
        *value = INT_MIN;
    } else if (is_negative) {
        *value = -(int)magnitude;
    } else {
        *value = (int)magnitude;
    }
    return 1;
}

int main(void) {
    size_t count = 0;
    if (!read_count(&count)) {
        fputs("error: expected an element count\n", stderr);
        return 1;
    }

    int_vector *values = int_vector_create();
    if (values == NULL) {
        fputs("error: unable to create integer vector\n", stderr);
        return 1;
    }

    for (size_t index = 0; index < count; ++index) {
        int value = 0;
        if (!read_integer(&value)) {
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
