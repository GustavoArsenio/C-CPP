#include <stdio.h>

#define VALUE_COUNT 15
#define BUFFER_SIZE 5

static void print_buffer(const char *name, const int values[], int count) {
    for (int i = 0; i < count; ++i) {
        printf("%s[%d] = %d\n", name, i, values[i]);
    }
}

int main(void) {
    int evens[BUFFER_SIZE];
    int odds[BUFFER_SIZE];
    int even_count = 0;
    int odd_count = 0;

    for (int i = 0; i < VALUE_COUNT; ++i) {
        int value;
        if (scanf("%d", &value) != 1) {
            fputs("Expected 15 integers.\n", stderr);
            return 1;
        }
        if (value % 2 == 0) {
            evens[even_count++] = value;
            if (even_count == BUFFER_SIZE) {
                print_buffer("par", evens, even_count);
                even_count = 0;
            }
        } else {
            odds[odd_count++] = value;
            if (odd_count == BUFFER_SIZE) {
                print_buffer("impar", odds, odd_count);
                odd_count = 0;
            }
        }
    }

    print_buffer("impar", odds, odd_count);
    print_buffer("par", evens, even_count);
    return 0;
}
