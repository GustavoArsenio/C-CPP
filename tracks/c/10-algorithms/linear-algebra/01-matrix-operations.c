#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix-operations.h"

#ifndef MATRIX_OPERATIONS_NO_MAIN
static bool read_matrix(int rows, int columns,
                        int32_t matrix[MAX_DIMENSION][MAX_DIMENSION]);
static bool run_collection(void);
#endif

void free_collection(struct matrix_record *head) {
    while (head != NULL) {
        struct matrix_record *next = head->next;
        free(head);
        head = next;
    }
}

#ifndef MATRIX_OPERATIONS_NO_MAIN
static bool read_matrix(int rows, int columns,
                        int32_t matrix[MAX_DIMENSION][MAX_DIMENSION]) {
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            if (scanf("%" SCNd32, &matrix[row][column]) != 1) {
                return false;
            }
        }
    }
    return true;
}
#endif

bool add_matrices(int rows, int columns,
                  int32_t left[MAX_DIMENSION][MAX_DIMENSION],
                  int32_t right[MAX_DIMENSION][MAX_DIMENSION],
                  int64_t result[MAX_DIMENSION][MAX_DIMENSION]) {
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            result[row][column] =
                (int64_t)left[row][column] + (int64_t)right[row][column];
        }
    }
    return true;
}

bool multiply_matrices(int rows, int shared, int columns,
                       int32_t left[MAX_DIMENSION][MAX_DIMENSION],
                       int32_t right[MAX_DIMENSION][MAX_DIMENSION],
                       int64_t result[MAX_DIMENSION][MAX_DIMENSION]) {
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            int64_t sum = 0;
            for (int index = 0; index < shared; ++index) {
                int64_t product =
                    (int64_t)left[row][index] * (int64_t)right[index][column];
                if ((product > 0 && sum > INT64_MAX - product) ||
                    (product < 0 && sum < INT64_MIN - product)) {
                    return false;
                }
                sum += product;
            }
            result[row][column] = sum;
        }
    }
    return true;
}

uint64_t factorial(unsigned int value) {
    uint64_t result = 1;
    for (unsigned int factor = 2; factor <= value; ++factor) {
        result *= factor;
    }
    return result;
}

/*
 * The Leibniz formula enumerates at most 8! products. Each product has
 * magnitude at most 2^248; even the sum of all absolute terms is below
 * 8! * 2^248 < 2^264. Nine base-2^32 limbs therefore hold every partial sum.
 * All determinant arithmetic is integer arithmetic, so no rounding occurs.
 */
enum { DETERMINANT_LIMBS = 9 };
struct big_signed {
    uint32_t limbs[DETERMINANT_LIMBS];
    size_t used;
    int sign;
};

static void big_normalize(struct big_signed *value) {
    while (value->used > 0 && value->limbs[value->used - 1] == 0) {
        --value->used;
    }
    if (value->used == 0) value->sign = 0;
}

static void big_multiply_small(struct big_signed *value, uint32_t factor) {
    if (factor == 0 || value->sign == 0) {
        value->used = 0;
        value->sign = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < value->used; ++i) {
        uint64_t product = (uint64_t)value->limbs[i] * factor + carry;
        value->limbs[i] = (uint32_t)product;
        carry = product >> 32;
    }
    if (carry != 0 && value->used < DETERMINANT_LIMBS) {
        value->limbs[value->used++] = (uint32_t)carry;
    }
}

static int big_compare_magnitude(const struct big_signed *left,
                                 const struct big_signed *right) {
    if (left->used != right->used) return left->used > right->used ? 1 : -1;
    for (size_t i = left->used; i > 0; --i) {
        if (left->limbs[i - 1] != right->limbs[i - 1]) {
            return left->limbs[i - 1] > right->limbs[i - 1] ? 1 : -1;
        }
    }
    return 0;
}

static void big_add_magnitude(struct big_signed *left,
                              const struct big_signed *right) {
    uint64_t carry = 0;
    size_t limit = left->used > right->used ? left->used : right->used;
    for (size_t i = 0; i < limit; ++i) {
        uint64_t sum = carry;
        if (i < left->used) sum += left->limbs[i];
        if (i < right->used) sum += right->limbs[i];
        left->limbs[i] = (uint32_t)sum;
        carry = sum >> 32;
    }
    left->used = limit;
    if (carry != 0 && left->used < DETERMINANT_LIMBS) {
        left->limbs[left->used++] = (uint32_t)carry;
    }
}

static void big_subtract_magnitude(struct big_signed *left,
                                   const struct big_signed *right) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < left->used; ++i) {
        uint64_t subtrahend = (i < right->used ? right->limbs[i] : 0) + borrow;
        uint64_t current = left->limbs[i];
        left->limbs[i] = (uint32_t)(current - subtrahend);
        borrow = current < subtrahend;
    }
    big_normalize(left);
}

static void big_add_signed(struct big_signed *total,
                           const struct big_signed *term) {
    if (term->sign == 0) return;
    if (total->sign == 0) {
        *total = *term;
    } else if (total->sign == term->sign) {
        big_add_magnitude(total, term);
    } else {
        int comparison = big_compare_magnitude(total, term);
        if (comparison == 0) {
            total->used = 0;
            total->sign = 0;
        } else if (comparison > 0) {
            big_subtract_magnitude(total, term);
        } else {
            struct big_signed difference = *term;
            big_subtract_magnitude(&difference, total);
            *total = difference;
        }
    }
}

static void determinant_permutations(
    int size, int row, int32_t matrix[MAX_DIMENSION][MAX_DIMENSION],
    bool used[MAX_DIMENSION], int permutation[MAX_DIMENSION],
    struct big_signed *total) {
    if (row == size) {
        struct big_signed term = {{0}, 1, 1};
        term.limbs[0] = 1;
        int parity = 0;
        for (int i = 0; i < size; ++i) {
            int32_t item = matrix[i][permutation[i]];
            if (item < 0) term.sign = -term.sign;
            uint32_t magnitude =
                item < 0 ? (uint32_t)(-(int64_t)item) : (uint32_t)item;
            big_multiply_small(&term, magnitude);
            for (int j = i + 1; j < size; ++j) {
                if (permutation[i] > permutation[j]) ++parity;
            }
        }
        if (parity % 2 != 0) term.sign = -term.sign;
        big_add_signed(total, &term);
        return;
    }
    for (int column = 0; column < size; ++column) {
        if (used[column]) continue;
        used[column] = true;
        permutation[row] = column;
        determinant_permutations(size, row + 1, matrix, used, permutation,
                                 total);
        used[column] = false;
    }
}

bool determinant_decimal(int size, int32_t matrix[MAX_DIMENSION][MAX_DIMENSION],
                         char output[DETERMINANT_TEXT_SIZE]) {
    if (size < 1 || size > MAX_DIMENSION || matrix == NULL || output == NULL) {
        return false;
    }

    struct big_signed value = {{0}, 0, 0};
    bool used[MAX_DIMENSION] = {false};
    int permutation[MAX_DIMENSION] = {0};
    determinant_permutations(size, 0, matrix, used, permutation, &value);

    uint32_t decimal_work[DETERMINANT_LIMBS];
    for (size_t i = 0; i < value.used; ++i) decimal_work[i] = value.limbs[i];
    size_t work_used = value.used;
    char reversed[DETERMINANT_TEXT_SIZE];
    size_t digits = 0;
    while (work_used > 0) {
        uint64_t remainder = 0;
        for (size_t i = work_used; i > 0; --i) {
            uint64_t current = (remainder << 32) | decimal_work[i - 1];
            decimal_work[i - 1] = (uint32_t)(current / 10);
            remainder = current % 10;
        }
        reversed[digits++] = (char)('0' + remainder);
        while (work_used > 0 && decimal_work[work_used - 1] == 0) --work_used;
    }
    if (digits == 0) reversed[digits++] = '0';
    size_t output_index = 0;
    if (value.sign < 0) output[output_index++] = '-';
    while (digits > 0) output[output_index++] = reversed[--digits];
    output[output_index] = '\0';
    return true;
}

#ifndef MATRIX_OPERATIONS_NO_MAIN
static bool run_collection(void) {
    int count;
    struct matrix_record *head = NULL;
    struct matrix_record **tail = &head;
    if (scanf("%d", &count) != 1 || count < 0 || count > 1000) return false;
    for (int index = 0; index < count; ++index) {
        struct matrix_record *record = calloc(1, sizeof(*record));
        if (record == NULL) {
            free_collection(head);
            return false;
        }
        if (scanf("%31s %d %d", record->name, &record->rows,
                  &record->columns) != 3 ||
            record->rows < 1 || record->rows > MAX_DIMENSION ||
            record->columns < 1 || record->columns > MAX_DIMENSION ||
            !read_matrix(record->rows, record->columns, record->values)) {
            free(record);
            free_collection(head);
            return false;
        }
        *tail = record;
        tail = &record->next;
    }
    int selected;
    if (scanf("%d", &selected) != 1 || selected < -4 || selected > count) {
        free_collection(head);
        return false;
    }
    if (selected < 0) {
        int first_position;
        if (scanf("%d", &first_position) != 1 || first_position < 1 ||
            first_position > count) {
            free_collection(head);
            return false;
        }
        struct matrix_record *first = head;
        for (int i = 1; i < first_position; ++i) first = first->next;
        if (selected == -3) {
            for (int column = 0; column < first->columns; ++column) {
                for (int row = 0; row < first->rows; ++row) {
                    printf("%s%" PRId32, row == 0 ? "" : " ",
                           first->values[row][column]);
                }
                putchar('\n');
            }
        } else if (selected == -4) {
            char text[DETERMINANT_TEXT_SIZE];
            if (first->rows != first->columns) {
                free_collection(head);
                return false;
            }
            if (!determinant_decimal(first->rows, first->values, text)) {
                free_collection(head);
                return false;
            }
            printf("%s\n", text);
        } else {
            int second_position;
            if (scanf("%d", &second_position) != 1 || second_position < 1 ||
                second_position > count) {
                free_collection(head);
                return false;
            }
            struct matrix_record *second = head;
            for (int i = 1; i < second_position; ++i) second = second->next;
            int result_rows, result_columns;
            if (selected == -1) {
                if (first->rows != second->rows ||
                    first->columns != second->columns) {
                    free_collection(head);
                    return false;
                }
                result_rows = first->rows;
                result_columns = first->columns;
            } else {
                if (first->columns != second->rows) {
                    free_collection(head);
                    return false;
                }
                result_rows = first->rows;
                result_columns = second->columns;
            }
            struct matrix_record *result = calloc(1, sizeof(*result));
            if (result == NULL) {
                free_collection(head);
                return false;
            }
            result->rows = result_rows;
            result->columns = result_columns;
            snprintf(result->name, sizeof(result->name),
                     selected == -1 ? "sum" : "product");
            int64_t values[MAX_DIMENSION][MAX_DIMENSION] = {{0}};
            bool valid =
                selected == -1
                    ? add_matrices(result_rows, result_columns, first->values,
                                   second->values, values)
                    : multiply_matrices(result_rows, first->columns,
                                        result_columns, first->values,
                                        second->values, values);
            for (int row = 0; valid && row < result_rows; ++row) {
                for (int column = 0; column < result_columns; ++column) {
                    if (values[row][column] < INT32_MIN ||
                        values[row][column] > INT32_MAX) {
                        valid = false;
                        break;
                    }
                    result->values[row][column] = (int32_t)values[row][column];
                }
            }
            if (!valid) {
                free(result);
                free_collection(head);
                return false;
            }
            *tail = result;
            printf("%d %s %d %d\n", count + 1, result->name, result->rows,
                   result->columns);
            for (int row = 0; row < result->rows; ++row) {
                for (int column = 0; column < result->columns; ++column) {
                    printf("%s%" PRId32, column == 0 ? "" : " ",
                           result->values[row][column]);
                }
                putchar('\n');
            }
        }
        free_collection(head);
        return true;
    }
    int position = 1;
    for (const struct matrix_record *record = head; record != NULL;
         record = record->next, ++position) {
        if (selected != 0 && selected != position) continue;
        printf("%d %s %d %d\n", position, record->name, record->rows,
               record->columns);
        for (int row = 0; row < record->rows; ++row) {
            for (int column = 0; column < record->columns; ++column) {
                printf("%s%" PRId32, column == 0 ? "" : " ",
                       record->values[row][column]);
            }
            putchar('\n');
        }
    }
    free_collection(head);
    return true;
}
#endif

#ifndef MATRIX_OPERATIONS_NO_MAIN
int main(void) {
    int operation;
    int rows, shared, columns;
    int32_t left[MAX_DIMENSION][MAX_DIMENSION] = {{0}};
    int32_t right[MAX_DIMENSION][MAX_DIMENSION] = {{0}};
    int64_t result[MAX_DIMENSION][MAX_DIMENSION] = {{0}};

    if (scanf("%d", &operation) != 1) {
        return 1;
    }
    if (operation == 1) {
        if (scanf("%d %d", &rows, &columns) != 2 || rows < 1 ||
            rows > MAX_DIMENSION || columns < 1 || columns > MAX_DIMENSION ||
            !read_matrix(rows, columns, left) ||
            !read_matrix(rows, columns, right)) {
            return 1;
        }
        add_matrices(rows, columns, left, right, result);
        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                printf("%s%" PRId64, column == 0 ? "" : " ",
                       result[row][column]);
            }
            putchar('\n');
        }
    } else if (operation == 2) {
        if (scanf("%d %d %d", &rows, &shared, &columns) != 3 || rows < 1 ||
            rows > MAX_DIMENSION || shared < 1 || shared > MAX_DIMENSION ||
            columns < 1 || columns > MAX_DIMENSION ||
            !read_matrix(rows, shared, left) ||
            !read_matrix(shared, columns, right)) {
            return 1;
        }
        if (!multiply_matrices(rows, shared, columns, left, right, result)) {
            fputs("Matrix product exceeds the supported integer range.\n",
                  stderr);
            return 1;
        }
        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                printf("%s%" PRId64, column == 0 ? "" : " ",
                       result[row][column]);
            }
            putchar('\n');
        }
    } else if (operation == 3) {
        if (scanf("%d %d", &rows, &columns) != 2 || rows < 1 ||
            rows > MAX_DIMENSION || columns < 1 || columns > MAX_DIMENSION ||
            !read_matrix(rows, columns, left)) {
            return 1;
        }
        for (int column = 0; column < columns; ++column) {
            for (int row = 0; row < rows; ++row) {
                printf("%s%" PRId32, row == 0 ? "" : " ", left[row][column]);
            }
            putchar('\n');
        }
    } else if (operation == 4) {
        unsigned int value;
        if (scanf("%u", &value) != 1 || value > 20) {
            return 1;
        }
        printf("%" PRIu64 "\n", factorial(value));
    } else if (operation == 5) {
        if (scanf("%d", &rows) != 1 || rows < 1 || rows > MAX_DIMENSION ||
            !read_matrix(rows, rows, left))
            return 1;
        char determinant_text[DETERMINANT_TEXT_SIZE];
        if (!determinant_decimal(rows, left, determinant_text)) return 1;
        printf("%s\n", determinant_text);
    } else if (operation == 6) {
        if (!run_collection()) return 1;
    } else {
        return 1;
    }
    return 0;
}
#endif
