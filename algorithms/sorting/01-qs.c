/* 01-qs.c - qsort example
 *  Purpose:  Sort an array using qsort
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int compare_ints(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    int values[] = {5, 3, 8, 1, 4};
    size_t count = sizeof(values) / sizeof(values[0]);

    qsort(values, count, sizeof(values[0]), compare_ints);
    printf("Sorted:");
    for (size_t index = 0; index < count; ++index) {
        printf(" %d", values[index]);
    }
    putchar('\n');
    return 0;
}
