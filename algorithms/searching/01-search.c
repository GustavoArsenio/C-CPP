/* 01‑search.c – Linear search example
 *  Author:  [Your name]
 *  Purpose:  Search for a value in an array
 *  Compile:   gcc -Wall -Wextra -O2 01-search.c -o search
 *  Run:       ./search
 */

#include <stddef.h>
#include <stdio.h>

int main(void) {
    int arr[] = {10, 20, 30, 40, 50};
    int target = 30;
    int found = 0;
    for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
        if (arr[i] == target) {
            found = 1;
            break;
        }
    }
    printf("%s\n", found ? "found" : "not found");
    return 0;
}
