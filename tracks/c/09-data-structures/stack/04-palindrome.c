#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "char_stack.h"

#define LINE_CAPACITY 256

int main(void) {
    char line[LINE_CAPACITY];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fputs("Expected one line of text.\n", stderr);
        return 1;
    }

    const size_t length = strcspn(line, "\n");
    if (length == sizeof(line) - 1) {
        int character = getchar();
        if (character != '\n' && character != EOF) {
            while ((character = getchar()) != '\n' && character != EOF) {
            }
            fputs("Input line is too long.\n", stderr);
            return 1;
        }
    }

    struct char_stack *characters = char_stack_create();
    if (characters == NULL) {
        fputs("Unable to create character stack.\n", stderr);
        return 1;
    }
    for (size_t i = 0; i < length; ++i) {
        if (!char_stack_push(characters, line[i])) {
            fputs("Unable to store input character.\n", stderr);
            char_stack_destroy(characters);
            return 1;
        }
    }

    bool is_palindrome = true;
    for (size_t i = 0; i < length; ++i) {
        char reversed_character;
        if (!char_stack_pop(characters, &reversed_character) ||
            (unsigned char)reversed_character != (unsigned char)line[i]) {
            is_palindrome = false;
            break;
        }
    }

    puts(is_palindrome ? "palindrome" : "not palindrome");
    char_stack_destroy(characters);
    return 0;
}
