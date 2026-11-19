#include <stdio.h>

#define ARRAY_LENGTH 50

int main(void) {
    double first[ARRAY_LENGTH];
    double second[ARRAY_LENGTH];
    double averages[ARRAY_LENGTH];

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        if (scanf("%lf", &first[i]) != 1) {
            fputs("Expected 50 values for the first array.\n", stderr);
            return 1;
        }
    }
    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        if (scanf("%lf", &second[i]) != 1) {
            fputs("Expected 50 values for the second array.\n", stderr);
            return 1;
        }
        averages[i] = (first[i] + second[i]) / 2.0;
    }

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        printf("%.2f\n", averages[i]);
    }
    return 0;
}
