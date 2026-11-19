#include <stdio.h>

#define MATRIX_SIZE 12
#define LOWER_TRIANGLE_COUNT ((MATRIX_SIZE * (MATRIX_SIZE - 1)) / 2)

int main(void) {
    char operation;
    if (scanf(" %c", &operation) != 1 ||
        (operation != 'S' && operation != 'M')) {
        fputs("Expected operation S or M.\n", stderr);
        return 1;
    }

    double sum = 0.0;
    for (int row = 0; row < MATRIX_SIZE; ++row) {
        for (int column = 0; column < MATRIX_SIZE; ++column) {
            double value;
            if (scanf("%lf", &value) != 1) {
                fputs("Expected 144 matrix values.\n", stderr);
                return 1;
            }
            if (row > column) {
                sum += value;
            }
        }
    }

    if (operation == 'S') {
        printf("%.1f\n", sum);
    } else {
        printf("%.1f\n", sum / LOWER_TRIANGLE_COUNT);
    }
    return 0;
}
