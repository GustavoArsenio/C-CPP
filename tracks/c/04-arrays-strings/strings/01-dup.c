/* 01‑dup.c – String duplication example
 *  Author:  [Your name]
 *  Purpose:  Duplicate a string using strcpy
 *  Compile:   gcc -Wall -Wextra -O2 01-dup.c -o dup
 *  Run:       ./dup
 */

#include <stdio.h>
#include <string.h>

int main(void) {
    char src[] = "hello";
    char dst[100];
    strcpy(dst, src);
    printf("%s -> %s\n", src, dst);
    return 0;
}
