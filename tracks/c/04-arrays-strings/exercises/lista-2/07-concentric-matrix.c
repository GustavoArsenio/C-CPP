#include <stdio.h>
#include <stdlib.h>

#define MAX_MATRIX_SIZE 100

int main(void) {
    int size;
    while (scanf("%d", &size) == 1 && size != 0) {
        if (size < 1 || size > MAX_MATRIX_SIZE) {
            fprintf(stderr, "Expected a matrix size from 1 through %d.\n",
                    MAX_MATRIX_SIZE);
            return 1;
        }

        for (int row = 0; row < size; ++row) {
            for (int column = 0; column < size; ++column) {
                int layer = row;
                if (column < layer) layer = column;
                if (size - 1 - row < layer) layer = size - 1 - row;
                if (size - 1 - column < layer) layer = size - 1 - column;
                printf("%s%3d", column == 0 ? "" : " ", layer + 1);
            }
            putchar('\n');
        }
        putchar('\n');
    }
    return 0;
}
