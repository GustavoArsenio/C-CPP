/* 02‑while.c – While loop example
 *  Author:  [Your name]
 *  Purpose:  Count down from 10 to 0
 *  Compile:   gcc -Wall -Wextra -O2 02-while.c -o whileloop
 *  Run:       ./whileloop
 */

#include <stdio.h>

int main(void) {
    int i = 10;
    int sum = 0;
    while (i >= 0) {
        sum += i;
        --i;
    }
    printf("Sum 10..0 = %d\n", sum);
    return 0;
}
