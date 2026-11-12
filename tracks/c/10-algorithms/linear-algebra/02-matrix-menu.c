/* This session frontend links the shared matrix operation library. */
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix-operations.h"

enum { MAX_COLLECTION_SIZE = 1000, TOKEN_SIZE = 64 };

enum input_result { INPUT_ERROR = -1, INPUT_EOF = 0, INPUT_OK = 1 };

static enum input_result read_token(char token[TOKEN_SIZE]) {
    int character;
    size_t length = 0;
    do {
        character = getchar();
    } while (character != EOF && character <= ' ');
    if (character == EOF) return INPUT_EOF;
    do {
        if (length + 1 >= TOKEN_SIZE) {
            while (character != EOF && character > ' ') character = getchar();
            return INPUT_ERROR;
        }
        token[length++] = (char)character;
        character = getchar();
    } while (character != EOF && character > ' ');
    token[length] = '\0';
    return INPUT_OK;
}

static enum input_result read_integer(int64_t *value) {
    char token[TOKEN_SIZE];
    enum input_result status = read_token(token);
    if (status != INPUT_OK) return status;
    errno = 0;
    char *end = NULL;
    intmax_t parsed = strtoimax(token, &end, 10);
    if (errno == ERANGE || end == token || *end != '\0' || parsed < INT64_MIN ||
        parsed > INT64_MAX)
        return INPUT_ERROR;
    *value = (int64_t)parsed;
    return INPUT_OK;
}

static struct matrix_record *select_record(struct matrix_record *head,
                                           int position) {
    while (head != NULL && position > 1) {
        head = head->next;
        --position;
    }
    return position == 1 ? head : NULL;
}

static void print_record(const struct matrix_record *record, int position) {
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

static bool read_record(struct matrix_record ***tail, size_t *count) {
    char token[TOKEN_SIZE];
    int64_t rows, columns, value;
    enum input_result status = read_token(token);
    if (status != INPUT_OK ||
        strlen(token) >= sizeof(((struct matrix_record *)0)->name))
        return false;
    struct matrix_record *record = calloc(1, sizeof(*record));
    if (record == NULL) return false;
    strcpy(record->name, token);
    if (read_integer(&rows) != INPUT_OK || read_integer(&columns) != INPUT_OK ||
        rows < 1 || rows > MAX_DIMENSION || columns < 1 ||
        columns > MAX_DIMENSION) {
        free(record);
        return false;
    }
    record->rows = (int)rows;
    record->columns = (int)columns;
    for (int row = 0; row < record->rows; ++row) {
        for (int column = 0; column < record->columns; ++column) {
            if (read_integer(&value) != INPUT_OK || value < INT32_MIN ||
                value > INT32_MAX) {
                free(record);
                return false;
            }
            record->values[row][column] = (int32_t)value;
        }
    }
    **tail = record;
    *tail = &record->next;
    ++*count;
    return true;
}

static bool append_result(struct matrix_record ***tail,
                          const struct matrix_record *left,
                          const struct matrix_record *right, int operation,
                          size_t position) {
    int rows, columns;
    if (operation == 2) {
        if (left->rows != right->rows || left->columns != right->columns)
            return false;
        rows = left->rows;
        columns = left->columns;
    } else {
        if (left->columns != right->rows) return false;
        rows = left->rows;
        columns = right->columns;
    }
    struct matrix_record *result = calloc(1, sizeof(*result));
    if (result == NULL) return false;
    result->rows = rows;
    result->columns = columns;
    snprintf(result->name, sizeof(result->name), "%s",
             operation == 2 ? "sum" : "product");
    int64_t values[MAX_DIMENSION][MAX_DIMENSION] = {{0}};
    bool valid =
        operation == 2
            ? add_matrices(rows, columns,
                           (int32_t(*)[MAX_DIMENSION])left->values,
                           (int32_t(*)[MAX_DIMENSION])right->values, values)
            : multiply_matrices(rows, left->columns, right->columns,
                                (int32_t(*)[MAX_DIMENSION])left->values,
                                (int32_t(*)[MAX_DIMENSION])right->values,
                                values);
    for (int row = 0; valid && row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
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
        return false;
    }
    **tail = result;
    *tail = &result->next;
    print_record(result, (int)position);
    return true;
}

int main(void) {
    struct matrix_record *head = NULL;
    struct matrix_record **tail = &head;
    size_t count = 0;
    int exit_status = 0;
    for (;;) {
        int64_t command;
        puts(
            "0 exit | 1 add | 2 sum | 3 multiply | 4 factorial |"
            " 5 determinant | 6 transpose | 7 display");
        enum input_result input = read_integer(&command);
        if (input == INPUT_EOF) break;
        if (input == INPUT_ERROR) {
            exit_status = 1;
            break;
        }
        if (command == 0) break;
        if (command < 0 || command > 7) {
            puts("Invalid option");
            continue;
        }
        if (command == 1) {
            if (count == MAX_COLLECTION_SIZE || !read_record(&tail, &count)) {
                fputs("Invalid matrix input or collection full\n", stderr);
                exit_status = 1;
                break;
            }
            printf("Added matrix %zu\n", count);
            continue;
        }
        if (command == 4) {
            int64_t number;
            if (read_integer(&number) != INPUT_OK) {
                fputs("Incomplete or malformed command input\n", stderr);
                exit_status = 1;
                break;
            }
            if (number < 0 || number > 20) {
                puts("Invalid factorial input");
                continue;
            }
            printf("%" PRIu64 "\n", factorial((unsigned int)number));
            continue;
        }
        if (command == 2 || command == 3) {
            int64_t first, second;
            if (read_integer(&first) != INPUT_OK ||
                read_integer(&second) != INPUT_OK) {
                fputs("Incomplete or malformed command input\n", stderr);
                exit_status = 1;
                break;
            }
            struct matrix_record *left = first > 0 && first <= (int64_t)count
                                             ? select_record(head, (int)first)
                                             : NULL;
            struct matrix_record *right = second > 0 && second <= (int64_t)count
                                              ? select_record(head, (int)second)
                                              : NULL;
            if (left == NULL || right == NULL || count == MAX_COLLECTION_SIZE ||
                !append_result(&tail, left, right, (int)command, count + 1)) {
                puts("Invalid selection, dimensions, or result range");
            } else {
                ++count;
            }
            continue;
        }
        int64_t selected;
        if (read_integer(&selected) != INPUT_OK) {
            fputs("Incomplete or malformed command input\n", stderr);
            exit_status = 1;
            break;
        }
        if (command == 7 && selected == 0) {
            int position = 1;
            for (struct matrix_record *item = head; item != NULL;
                 item = item->next, ++position)
                print_record(item, position);
            continue;
        }
        struct matrix_record *record =
            selected > 0 && selected <= (int64_t)count
                ? select_record(head, (int)selected)
                : NULL;
        if (record == NULL) {
            puts("Invalid matrix selection");
            continue;
        }
        if (command == 5) {
            if (record->rows != record->columns) {
                puts("Determinant requires a square matrix");
                continue;
            }
            char determinant[DETERMINANT_TEXT_SIZE];
            determinant_decimal(record->rows, record->values, determinant);
            printf("%s\n", determinant);
        } else if (command == 6) {
            for (int column = 0; column < record->columns; ++column) {
                for (int row = 0; row < record->rows; ++row)
                    printf("%s%" PRId32, row == 0 ? "" : " ",
                           record->values[row][column]);
                putchar('\n');
            }
        } else {
            print_record(record, (int)selected);
        }
    }
    free_collection(head);
    return exit_status;
}
