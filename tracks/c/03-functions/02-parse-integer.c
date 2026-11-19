#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum parse_status { PARSE_OK, PARSE_INVALID, PARSE_OUT_OF_RANGE };

static enum parse_status parse_integer(const char *input, int *value) {
    size_t first_digit = 0;
    if (input[0] == '+' || input[0] == '-') {
        first_digit = 1;
    }
    if (input[first_digit] == '\0') {
        return PARSE_INVALID;
    }

    for (size_t index = first_digit; input[index] != '\0'; ++index) {
        if (input[index] < '0' || input[index] > '9') {
            return PARSE_INVALID;
        }
    }

    errno = 0;
    char *end = NULL;
    const intmax_t parsed = strtoimax(input, &end, 10);
    if (end == input || *end != '\0') {
        return PARSE_INVALID;
    }
    if (errno == ERANGE || parsed < INT_MIN || parsed > INT_MAX) {
        return PARSE_OUT_OF_RANGE;
    }

    *value = (int)parsed;
    return PARSE_OK;
}

static int read_line(char *buffer, size_t capacity, size_t *length) {
    size_t used = 0;
    int too_long = 0;
    int invalid_byte = 0;
    int character = 0;

    while ((character = fgetc(stdin)) != EOF && character != '\n') {
        if (character == '\0') {
            invalid_byte = 1;
        }
        if (used < capacity - 1) {
            buffer[used++] = (char)character;
        } else {
            too_long = 1;
        }
    }
    if (ferror(stdin) || (character == EOF && used == 0 && !too_long)) {
        return 0;
    }

    buffer[used] = '\0';
    *length = used;
    return !too_long && !invalid_byte;
}

int main(void) {
    char input[129];
    size_t length = 0;
    if (!read_line(input, sizeof input, &length) || length == 0) {
        puts("error: invalid integer");
        return 1;
    }

    int value = 0;
    const enum parse_status status = parse_integer(input, &value);
    if (status == PARSE_INVALID) {
        puts("error: invalid integer");
        return 1;
    }
    if (status == PARSE_OUT_OF_RANGE) {
        puts("error: integer out of range");
        return 1;
    }

    printf("value=%d\n", value);
    return 0;
}
