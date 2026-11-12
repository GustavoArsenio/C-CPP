/* 01‑input.c – Basic I/O example
 *  Author:  [Your name]
 *  Purpose:  Read an integer and echo it
 *  Compile:   gcc -Wall -Wextra -O2 01-input.c -o input
 *  Run:       ./input
 */

#include <stdio.h>

int main(void) {
    int x;
    printf("Enter a number: ");
    if (scanf("%d", &x) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    printf("You entered %d\n", x);
    return 0;
}
