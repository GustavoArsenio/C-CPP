/* 01‑ptr.c – Pointer example
 *  Author:  [Your name]
 *  Purpose:  Show pointer dereference
 *  Compile:   gcc -Wall -Wextra -O2 01-ptr.c -o ptr
 *  Run:       ./ptr
 */

#include <stdio.h>

int main(void) {
    int val = 42;
    int *ptr = &val;
    printf("value %d via pointer %d\n", val, *ptr);
    return 0;
}
