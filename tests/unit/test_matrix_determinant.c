#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix-operations.h"

#define CHECK(condition)                                                       \
    do {                                                                       \
        if (!(condition)) {                                                    \
            fprintf(stderr, "check failed at %s:%d: %s\n", __FILE__, __LINE__, \
                    #condition);                                               \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

static void expect_determinant(int size,
                               int32_t matrix[MAX_DIMENSION][MAX_DIMENSION],
                               const char *expected) {
    char actual[DETERMINANT_TEXT_SIZE];
    CHECK(determinant_decimal(size, matrix, actual));
    CHECK(strcmp(actual, expected) == 0);
}

int main(void) {
    int32_t matrix[MAX_DIMENSION][MAX_DIMENSION] = {{0}};
    char unchanged[DETERMINANT_TEXT_SIZE] = "unchanged";
    CHECK(!determinant_decimal(0, matrix, unchanged));
    CHECK(strcmp(unchanged, "unchanged") == 0);
    CHECK(!determinant_decimal(-1, matrix, unchanged));
    CHECK(!determinant_decimal(MAX_DIMENSION + 1, matrix, unchanged));
    CHECK(!determinant_decimal(1, NULL, unchanged));
    CHECK(!determinant_decimal(1, matrix, NULL));
    CHECK(strcmp(unchanged, "unchanged") == 0);

    matrix[0][0] = 7;
    expect_determinant(1, matrix, "7");
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;
    expect_determinant(2, matrix, "-2");
    matrix[0][0] = 0;
    matrix[0][1] = 1;
    matrix[1][0] = 1;
    matrix[1][1] = 0;
    expect_determinant(2, matrix, "-1");
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[1][0] = 0;
    matrix[1][1] = 1;
    matrix[1][2] = 4;
    matrix[2][0] = 5;
    matrix[2][1] = 6;
    matrix[2][2] = 0;
    expect_determinant(3, matrix, "1");
    matrix[0][0] = 1;
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    matrix[0][3] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = 2;
    matrix[1][2] = 0;
    matrix[1][3] = 0;
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    matrix[2][2] = 3;
    matrix[2][3] = 0;
    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = 0;
    matrix[3][3] = 4;
    expect_determinant(4, matrix, "24");
    matrix[3][3] = 0;
    expect_determinant(4, matrix, "0");

    matrix[0][0] = INT32_MIN;
    matrix[0][1] = INT32_MAX;
    matrix[1][0] = INT32_MAX;
    matrix[1][1] = INT32_MIN;
    expect_determinant(2, matrix, "4294967295");

    /* Binary64 elimination rounds both products to the same value here,
     * although the exact integer determinant is one. */
    matrix[0][0] = INT32_C(2147483646);
    matrix[0][1] = INT32_C(2147483645);
    matrix[1][0] = INT32_C(2147483647);
    matrix[1][1] = INT32_C(2147483646);
    expect_determinant(2, matrix, "1");

    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            matrix[row][column] = row == column ? INT32_MAX : INT32_MIN;
        }
    }
    expect_determinant(8, matrix,
                       "-347376267172749653279217856191611407082245468776945900"
                       "913162254708176507109375");

    uint32_t state = 74830;
    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            state = state * UINT32_C(1664525) + UINT32_C(1013904223);
            int64_t signed_value = (int64_t)state - INT64_C(2147483648);
            matrix[row][column] = (int32_t)signed_value;
        }
    }
    expect_determinant(8, matrix,
                       "-157546896109958689553225351959002706123938274237013355"
                       "11555801798934528000");
    return 0;
}
