#include <stdio.h>

#define HALF_LENGTH 15
#define ARRAY_LENGTH (2 * HALF_LENGTH)

int main(void) {
    int first[HALF_LENGTH];
    int second[HALF_LENGTH];
    int merged[ARRAY_LENGTH];

    for (int i = 0; i < HALF_LENGTH; ++i) {
        if (scanf("%d", &first[i]) != 1) {
            fputs("Expected 15 integers for the first array.\n", stderr);
            return 1;
        }
        merged[i] = first[i];
    }
    for (int i = 0; i < HALF_LENGTH; ++i) {
        if (scanf("%d", &second[i]) != 1) {
            fputs("Expected 15 integers for the second array.\n", stderr);
            return 1;
        }
        merged[HALF_LENGTH + i] = second[i];
    }

    for (int i = 0; i < ARRAY_LENGTH; ++i) {
        printf("%s%d", i == 0 ? "" : " ", merged[i]);
    }
    putchar('\n');
    return 0;
}
