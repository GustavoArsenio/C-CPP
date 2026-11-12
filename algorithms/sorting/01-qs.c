/* 01‑qs.c – Quick‑sort example
 *  Author:  [Your name]
 *  Purpose:  Sort an array using qsort
 *  Compile:   gcc -Wall -Wextra -O2 01-qs.c -o qs
 *  Run:       ./qs
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    int arr[] = {5, 3, 8, 1, 4};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    qsort(arr, n, sizeof(int), cmp);
    printf("Sorted: ");
    for (size_t i = 0; i < n; ++i) printf("%d ", arr[i]);
    printf("\n");
    return 0;
}
