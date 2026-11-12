#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum { MAX_DIMENSION = 8, DETERMINANT_TEXT_SIZE = 96 };

struct matrix_record {
    char name[32];
    int rows;
    int columns;
    int32_t values[MAX_DIMENSION][MAX_DIMENSION];
    struct matrix_record *next;
};

void free_collection(struct matrix_record *head);
bool add_matrices(int rows, int columns,
                  int32_t left[MAX_DIMENSION][MAX_DIMENSION],
                  int32_t right[MAX_DIMENSION][MAX_DIMENSION],
                  int64_t result[MAX_DIMENSION][MAX_DIMENSION]);
bool multiply_matrices(int rows, int shared, int columns,
                       int32_t left[MAX_DIMENSION][MAX_DIMENSION],
                       int32_t right[MAX_DIMENSION][MAX_DIMENSION],
                       int64_t result[MAX_DIMENSION][MAX_DIMENSION]);
uint64_t factorial(unsigned int value);
void determinant_decimal(int size, int32_t matrix[MAX_DIMENSION][MAX_DIMENSION],
                         char output[DETERMINANT_TEXT_SIZE]);

#endif
