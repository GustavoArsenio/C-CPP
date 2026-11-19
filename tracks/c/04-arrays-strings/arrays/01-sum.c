/* 01‑sum.c – Sum of array
 *  Author:  [Your name]
 *  Purpose:  Sum all elements in an array
 *  Compile:   gcc -Wall -Wextra -O2 01-sum.c -o sum
 *  Run:       ./sum
 */

#include <stddef.h>
#include <stdio.h>

int main(void) {
    int arr[] = {1, 2, 3, 4, 5};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    int sum = 0;
    for (size_t i = 0; i < n; ++i) sum += arr[i];
    printf("Sum = %d\n", sum);
    return 0;
}
