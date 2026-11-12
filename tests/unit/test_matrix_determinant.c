#include <assert.h>
#include <string.h>

#include "matrix-operations.h"

static void expect_determinant(int size,
                               int32_t matrix[MAX_DIMENSION][MAX_DIMENSION],
                               const char *expected) {
    char actual[DETERMINANT_TEXT_SIZE];
    assert(determinant_decimal(size, matrix, actual));
    assert(strcmp(actual, expected) == 0);
}

int main(void) {
    int32_t matrix[MAX_DIMENSION][MAX_DIMENSION] = {{0}};
    char unchanged[DETERMINANT_TEXT_SIZE] = "unchanged";
    assert(!determinant_decimal(0, matrix, unchanged));
    assert(strcmp(unchanged, "unchanged") == 0);
    assert(!determinant_decimal(-1, matrix, unchanged));
    assert(!determinant_decimal(MAX_DIMENSION + 1, matrix, unchanged));
    assert(!determinant_decimal(1, NULL, unchanged));
    assert(!determinant_decimal(1, matrix, NULL));
    assert(strcmp(unchanged, "unchanged") == 0);

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
            matrix[row][column] =
                (int32_t)((int64_t)state - INT64_C(2147483648));
        }
    }
    expect_determinant(8, matrix,
                       "-157546896109958689553225351959002706123938274237013355"
                       "11555801798934528000");
    return 0;
}
