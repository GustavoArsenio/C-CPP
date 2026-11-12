#include <stdio.h>

#define VALUE_COUNT 100

int main(void) {
    double values[VALUE_COUNT];
    if (scanf("%lf", &values[0]) != 1) {
        fputs("Expected one numeric value.\n", stderr);
        return 1;
    }

    for (int i = 1; i < VALUE_COUNT; ++i) {
        values[i] = values[i - 1] / 2.0;
    }
    for (int i = 0; i < VALUE_COUNT; ++i) {
        printf("N[%d] = %.4f\n", i, values[i]);
    }
    return 0;
}
