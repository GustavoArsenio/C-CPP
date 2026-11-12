#include <stdio.h>

#define ARRAY_LENGTH 100

int main(void) {
    long long transformed[ARRAY_LENGTH];

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        int value;
        if (scanf("%d", &value) != 1) {
            fputs("Expected 100 integers.\n", stderr);
            return 1;
        }
        transformed[i] =
            value % 2 == 0 ? (long long)value * 5 : (long long)value + 5;
    }

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        printf("%lld\n", transformed[i]);
    }
    return 0;
}
