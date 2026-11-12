/* Count logical lines in the file whose path is read from standard input. */
#include <stdint.h>
#include <stdio.h>

int main(void) {
    char path[1024];
    FILE *file;
    uintmax_t line_count = 0;
    int character;
    int has_content = 0;
    int last_character = '\n';

    if (fgets(path, sizeof(path), stdin) == NULL) {
        fputs("error: expected a file path on standard input\n", stderr);
        return 1;
    }

    for (size_t index = 0; path[index] != '\0'; ++index) {
        if (path[index] == '\n') {
            path[index] = '\0';
            break;
        }
        if (path[index] == '\r' && path[index + 1] == '\n') {
            path[index] = '\0';
            break;
        }
    }

    if (path[0] == '\0') {
        fputs("error: file path is empty\n", stderr);
        return 1;
    }
    if (path[0] != '\0' && path[sizeof(path) - 2] != '\0' &&
        path[sizeof(path) - 2] != '\n') {
        fputs("error: file path is too long\n", stderr);
        return 1;
    }

    file = fopen(path, "rb");
    if (file == NULL) {
        perror("error: cannot open file");
        return 1;
    }

    while ((character = fgetc(file)) != EOF) {
        has_content = 1;
        last_character = character;
        if (character == '\n') {
            ++line_count;
        }
    }

    if (ferror(file)) {
        fputs("error: failed while reading file\n", stderr);
        (void)fclose(file);
        return 1;
    }
    if (has_content && last_character != '\n') {
        ++line_count;
    }
    if (fclose(file) != 0) {
        fputs("error: failed to close file\n", stderr);
        return 1;
    }

    printf("%ju\n", line_count);
    return 0;
}
