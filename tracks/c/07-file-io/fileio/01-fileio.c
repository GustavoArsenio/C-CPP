/* 01‑fileio.c – Simple file I/O example
 *  Author:  [Your name]
 *  Purpose:  Read a file and print its contents
 *  Compile:   gcc -Wall -Wextra -O2 01-fileio.c -o fileio
 *  Run:       ./fileio < input.txt
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        fputs(line, stdout);
    }
    return 0;
}
