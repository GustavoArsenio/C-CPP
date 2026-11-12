/* Count logical lines in the file whose path is read from standard input. */
#include <stdint.h>
#include <stdio.h>

enum path_status {
    PATH_OK,
    PATH_MISSING,
    PATH_EMPTY,
    PATH_TOO_LONG,
    PATH_INVALID,
    PATH_READ_ERROR
};

static enum path_status read_path(char path[1024]) {
    size_t length = 0;
    int character;
    int saw_input = 0;
    int ended_with_newline = 0;
    int too_long = 0;
    int contains_nul = 0;

    while ((character = fgetc(stdin)) != EOF) {
        saw_input = 1;
        if (character == '\n') {
            ended_with_newline = 1;
            break;
        }
        if (character == '\0') {
            contains_nul = 1;
        }
        if (length < 1023) {
            path[length++] = (char)character;
        } else {
            too_long = 1;
        }
    }
    if (ferror(stdin)) {
        return PATH_READ_ERROR;
    }
    if (too_long) {
        return PATH_TOO_LONG;
    }
    if (ended_with_newline && length > 0 && path[length - 1] == '\r') {
        --length;
    }
    if (contains_nul) {
        return PATH_INVALID;
    }
    if (length > 1022) {
        return PATH_TOO_LONG;
    }
    if (length == 0) {
        return saw_input ? PATH_EMPTY : PATH_MISSING;
    }
    path[length] = '\0';
    return PATH_OK;
}

int main(void) {
    char path[1024];
    FILE *file;
    uintmax_t line_count = 0;
    int character;
    int has_content = 0;
    int last_character = '\n';
    enum path_status path_result = read_path(path);

    if (path_result == PATH_MISSING) {
        fputs("error: expected a file path on standard input\n", stderr);
        return 1;
    }
    if (path_result == PATH_EMPTY) {
        fputs("error: file path is empty\n", stderr);
        return 1;
    }
    if (path_result == PATH_TOO_LONG) {
        fputs("error: file path is too long\n", stderr);
        return 1;
    }
    if (path_result == PATH_INVALID) {
        fputs("error: file path contains a null byte\n", stderr);
        return 1;
    }
    if (path_result == PATH_READ_ERROR) {
        fputs("error: failed while reading file path\n", stderr);
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
            if (line_count == UINTMAX_MAX) {
                fputs("error: line count overflow\n", stderr);
                (void)fclose(file);
                return 1;
            }
            ++line_count;
        }
    }

    if (ferror(file)) {
        fputs("error: failed while reading file\n", stderr);
        (void)fclose(file);
        return 1;
    }
    if (has_content && last_character != '\n') {
        if (line_count == UINTMAX_MAX) {
            fputs("error: line count overflow\n", stderr);
            (void)fclose(file);
            return 1;
        }
        ++line_count;
    }
    if (fclose(file) != 0) {
        fputs("error: failed to close file\n", stderr);
        return 1;
    }

    printf("%ju\n", line_count);
    return 0;
}
