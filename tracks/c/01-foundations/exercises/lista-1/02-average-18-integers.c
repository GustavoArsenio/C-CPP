#include <stdio.h>

#define VALUE_COUNT 18

int main(void) {
    long long sum = 0;

    for (int i = 0; i < VALUE_COUNT; ++i) {
        int value;
        if (scanf("%d", &value) != 1) {
            fputs("Expected 18 integers.\n", stderr);
            return 1;
        }
        sum += value;
    }

    printf("Average: %.2f\n", (double)sum / VALUE_COUNT);
    return 0;
}
