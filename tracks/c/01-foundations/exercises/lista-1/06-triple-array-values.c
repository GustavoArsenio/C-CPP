#include <stdio.h>

#define ARRAY_LENGTH 30

int main(void) {
    double values[ARRAY_LENGTH];

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        if (scanf("%lf", &values[i]) != 1) {
            fputs("Expected 30 numeric values.\n", stderr);
            return 1;
        }
    }

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        printf("%.2f\n", values[i] * 3.0);
    }
    return 0;
}
