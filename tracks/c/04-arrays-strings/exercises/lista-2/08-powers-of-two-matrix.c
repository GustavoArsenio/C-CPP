#include <stdint.h>
#include <stdio.h>

#define MAX_MATRIX_SIZE 15

int main(void) {
    int size;
    while (scanf("%d", &size) == 1 && size != 0) {
        if (size < 1 || size > MAX_MATRIX_SIZE) {
            fprintf(stderr, "Expected a matrix size from 1 through %d.\n",
                    MAX_MATRIX_SIZE);
            return 1;
        }

        const unsigned int max_exponent = (unsigned int)(2 * (size - 1));
        const uint64_t max_value = UINT64_C(1) << max_exponent;
        int width = 1;
        for (uint64_t value = max_value; value >= 10; value /= 10) ++width;

        for (int row = 0; row < size; ++row) {
            for (int column = 0; column < size; ++column) {
                const unsigned int exponent = (unsigned int)(row + column);
                const uint64_t value = UINT64_C(1) << exponent;
                printf("%s%*llu", column == 0 ? "" : " ", width,
                       (unsigned long long)value);
            }
            putchar('\n');
        }
        putchar('\n');
    }
    return 0;
}
