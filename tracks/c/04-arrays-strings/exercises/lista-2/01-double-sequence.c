#include <limits.h>
#include <stdio.h>

#define VALUE_COUNT 10

int main(void) {
    long long values[VALUE_COUNT];
    if (scanf("%lld", &values[0]) != 1) {
        fputs("Expected one integer.\n", stderr);
        return 1;
    }
    if (values[0] >= 50) {
        fputs("Expected an input integer less than 50.\n", stderr);
        return 1;
    }

    for (int i = 1; i < VALUE_COUNT; ++i) {
        const long long previous = values[i - 1];
        if (previous > LLONG_MAX / 2 || previous < LLONG_MIN / 2) {
            fputs("Sequence exceeds the range of long long.\n", stderr);
            return 1;
        }
        values[i] = previous * 2;
    }

    for (int i = 0; i < VALUE_COUNT; ++i) {
        printf("N[%d] = %lld\n", i, values[i]);
    }
    return 0;
}
