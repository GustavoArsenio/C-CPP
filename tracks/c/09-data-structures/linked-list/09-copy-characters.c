#include <stdio.h>
#include <string.h>

#include "lista4_lists.h"

static void print_char(char value, void *context) {
    (void)context;
    putchar(value);
}

int main(void) {
    char input[4096];
    if (scanf("%4095s", input) != 1) {
        return 1;
    }
    struct char_list *source = char_list_create();
    struct char_list *iterative = NULL;
    struct char_list *recursive = NULL;
    if (source == NULL) {
        return 1;
    }
    for (size_t i = 0; i < strlen(input); ++i) {
        if (!char_list_append(source, input[i])) {
            char_list_destroy(source);
            return 1;
        }
    }
    iterative = char_list_copy_iterative(source);
    recursive = char_list_copy_recursive(source);
    if (iterative == NULL || recursive == NULL) {
        char_list_destroy(source);
        char_list_destroy(iterative);
        char_list_destroy(recursive);
        return 1;
    }
    char_list_visit(iterative, print_char, NULL);
    putchar('\n');
    char_list_visit(recursive, print_char, NULL);
    putchar('\n');
    char_list_destroy(source);
    char_list_destroy(iterative);
    char_list_destroy(recursive);
    return 0;
}
