#include <stdio.h>

#define ARRAY_LENGTH 20

int main(void) {
    double first[ARRAY_LENGTH];
    double second[ARRAY_LENGTH];

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        if (scanf("%lf", &first[i]) != 1) {
            fputs("Expected 20 values for the first array.\n", stderr);
            return 1;
        }
    }
    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        if (scanf("%lf", &second[i]) != 1) {
            fputs("Expected 20 values for the second array.\n", stderr);
            return 1;
        }
    }

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        const double temporary = first[i];
        first[i] = second[i];
        second[i] = temporary;
    }

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        printf("%.2f\n", first[i]);
    }
    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        printf("%.2f\n", second[i]);
    }
    return 0;
}
