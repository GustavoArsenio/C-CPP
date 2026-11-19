/* 01‑for-loop.c – For loop example
 *  Author:  [Your name]
 *  Purpose:  Sum 1..10
 *  Compile:   gcc -Wall -Wextra -O2 01-for-loop.c -o forloop
 *  Run:       ./forloop
 */

#include <stdio.h>

int main(void) {
    int sum = 0;
    for (int i = 1; i <= 10; ++i) {
        sum += i;
    }
    printf("Sum 1..10 = %d\n", sum);
    return 0;
}
