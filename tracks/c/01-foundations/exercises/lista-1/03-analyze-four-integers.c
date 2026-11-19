#include <stdbool.h>
#include <stdio.h>

#define VALUE_COUNT 4

int main(void) {
    long long odd_sum = 0;
    int odd_count = 0;
    int minimum = 0;
    int maximum = 0;
    int maximum_even = 0;
    bool has_value = false;
    bool has_even = false;

    for (int i = 0; i < VALUE_COUNT; ++i) {
        int value;
        if (scanf("%d", &value) != 1) {
            fputs("Expected four integers.\n", stderr);
            return 1;
        }

        if (!has_value) {
            minimum = value;
            maximum = value;
            has_value = true;
        } else {
            if (value < minimum) {
                minimum = value;
            }
            if (value > maximum) {
                maximum = value;
            }
        }

        if (value % 2 == 0) {
            if (!has_even || value > maximum_even) {
                maximum_even = value;
                has_even = true;
            }
        } else {
            odd_sum += value;
            ++odd_count;
        }
    }

    if (odd_count > 0) {
        printf("Odd average: %.2f\n", (double)odd_sum / odd_count);
    } else {
        puts("Odd average: unavailable");
    }
    if (has_even) {
        printf("Largest even: %d\n", maximum_even);
    } else {
        puts("Largest even: unavailable");
    }
    printf("Range: %lld\n", (long long)maximum - minimum);
    return 0;
}
