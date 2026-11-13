#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data-structures/hash-based/string_counter.h"

struct word_entry {
    const char *key;
    size_t count;
};

struct word_list {
    struct word_entry *entries;
    size_t size;
    size_t capacity;
};

static bool collect_word(const char *key, size_t count, void *context) {
    struct word_list *list = context;
    if (list->size == list->capacity) {
        size_t capacity = list->capacity == 0 ? 16 : list->capacity;
        if (capacity > SIZE_MAX / 2) {
            return false;
        }
        if (list->capacity != 0) {
            capacity *= 2;
        }
        if (capacity > SIZE_MAX / sizeof(*list->entries)) {
            return false;
        }

        struct word_entry *entries =
            realloc(list->entries, capacity * sizeof(*entries));
        if (entries == NULL) {
            return false;
        }
        list->entries = entries;
        list->capacity = capacity;
    }

    list->entries[list->size++] = (struct word_entry){key, count};
    return true;
}

static int compare_words(const void *left, const void *right) {
    const struct word_entry *a = left;
    const struct word_entry *b = right;
    return strcmp(a->key, b->key);
}

static bool is_space(int character) {
    return character == ' ' || character == '\t' || character == '\n' ||
           character == '\r' || character == '\v' || character == '\f';
}

/* Read one token beginning at first_character. The map owns a copy, so the
 * temporary input buffer can be released after incrementing the count. */
static bool read_word(struct string_counter *counter, int first_character) {
    char *buffer = NULL;
    size_t length = 0;
    size_t capacity = 0;
    int character = first_character;

    while (character != EOF && !is_space(character)) {
        if (character == '\0') {
            free(buffer);
            return false;
        }
        if (length == capacity) {
            size_t next_capacity = capacity == 0 ? 32 : capacity;
            if (capacity != 0) {
                if (capacity > (SIZE_MAX - 1) / 2) {
                    free(buffer);
                    return false;
                }
                next_capacity *= 2;
            }

            char *grown = realloc(buffer, next_capacity + 1);
            if (grown == NULL) {
                free(buffer);
                return false;
            }
            buffer = grown;
            capacity = next_capacity;
        }

        buffer[length++] = (char)character;
        character = getchar();
    }

    if (ferror(stdin)) {
        free(buffer);
        return false;
    }
    if (length == 0) {
        free(buffer);
        return false;
    }

    buffer[length] = '\0';
    size_t ignored_count;
    bool success = string_counter_increment(counter, buffer, &ignored_count);
    free(buffer);
    return success;
}

int main(void) {
    struct string_counter *counter = string_counter_create();
    struct word_list words = {0};
    int exit_code = EXIT_FAILURE;

    if (counter == NULL) {
        fputs("word-count: unable to allocate counter\n", stderr);
        return EXIT_FAILURE;
    }

    for (;;) {
        int character;
        do {
            character = getchar();
        } while (character != EOF && is_space(character));

        if (character == EOF) {
            if (ferror(stdin)) {
                fputs("word-count: input error\n", stderr);
                goto cleanup;
            }
            break;
        }

        if (!read_word(counter, character)) {
            fputs("word-count: input or allocation error\n", stderr);
            goto cleanup;
        }
    }

    if (!string_counter_visit(counter, collect_word, &words)) {
        fputs("word-count: unable to allocate output list\n", stderr);
        goto cleanup;
    }
    if (words.size > 1) {
        qsort(words.entries, words.size, sizeof(*words.entries), compare_words);
    }

    for (size_t index = 0; index < words.size; ++index) {
        if (printf("%s %zu\n", words.entries[index].key,
                   words.entries[index].count) < 0) {
            fputs("word-count: output error\n", stderr);
            goto cleanup;
        }
    }
    if (fflush(stdout) == EOF) {
        fputs("word-count: output error\n", stderr);
        goto cleanup;
    }

    exit_code = EXIT_SUCCESS;

cleanup:
    free(words.entries);
    string_counter_destroy(counter);
    return exit_code;
}
