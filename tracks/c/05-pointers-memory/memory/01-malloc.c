/* 01‑malloc.c – Dynamic allocation example
 *  Author:  [Your name]
 *  Purpose:  Allocate array via malloc and sum
 *  Compile:   gcc -Wall -Wextra -O2 01-malloc.c -o malloc
 *  Run:       ./malloc
 */

#include <malloc.h>
#include <stdio.h>

int main(void) {
    int n = 5;
    int *arr = malloc(n * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    int sum = 0;
    for (int i = 0; i < n; ++i) sum += arr[i];
    printf("Sum = %d\n", sum);
    free(arr);
    return 0;
}
