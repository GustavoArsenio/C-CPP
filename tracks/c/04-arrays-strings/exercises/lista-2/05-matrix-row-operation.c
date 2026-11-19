#include <stdio.h>

#define MATRIX_SIZE 12

int main(void) {
    int row;
    char operation;
    if (scanf("%d %c", &row, &operation) != 2 || row < 0 ||
        row >= MATRIX_SIZE || (operation != 'S' && operation != 'M')) {
        fprintf(stderr, "Expected a row from 0 to %d and S or M.\n",
                MATRIX_SIZE - 1);
        return 1;
    }

    double sum = 0.0;
    int included = 0;
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            double value;
            if (scanf("%lf", &value) != 1) {
                fputs("Expected 144 matrix values.\n", stderr);
                return 1;
            }
            if (i == row && j > row && j < MATRIX_SIZE - 1 - row) {
                sum += value;
                ++included;
            }
        }
    }

    if (operation == 'S') {
        printf("%.1f\n", sum);
    } else {
        printf("%.1f\n", included == 0 ? 0.0 : sum / included);
    }
    return 0;
}
