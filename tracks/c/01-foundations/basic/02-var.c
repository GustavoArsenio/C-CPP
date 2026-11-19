/* 02‑var.c – Variable declaration
 *  Author:  [Your name]
 *  Purpose:  Show declaration of basic types
 *  Compile:   gcc -Wall -Wextra -O2 02-var.c -o var
 *  Run:       ./var
 */

#include <stdio.h>

int main(void) {
    int a = 5;
    float b = 3.14f;
    double c = 2.718281828;
    printf("int %d, float %f, double %lf\n", a, b, c);
    return 0;
}
