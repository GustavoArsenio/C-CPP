#include <stdio.h>

#define MAX_FIBONACCI_INDEX 60

int main(void) {
    int test_cases;
    if (scanf("%d", &test_cases) != 1 || test_cases < 0) {
        fputs("Expected a non-negative test count.\n", stderr);
        return 1;
    }

    for (int test = 0; test < test_cases; ++test) {
        int index;
        if (scanf("%d", &index) != 1 || index < 0 ||
            index > MAX_FIBONACCI_INDEX) {
            fprintf(stderr, "Expected an index from 0 through %d.\n",
                    MAX_FIBONACCI_INDEX);
            return 1;
        }

        unsigned long long previous = 0;
        unsigned long long current = 1;
        for (int i = 2; i <= index; ++i) {
            const unsigned long long next = previous + current;
            previous = current;
            current = next;
        }
        printf("Fib(%d) = %llu\n", index, index == 0 ? previous : current);
    }
    return 0;
}
