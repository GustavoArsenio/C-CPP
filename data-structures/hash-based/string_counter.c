#include "string_counter.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum { INITIAL_CAPACITY = 16 };

struct string_counter_entry {
    char *key;
    uint64_t hash;
    size_t count;
};

struct string_counter {
    struct string_counter_entry *entries;
    size_t capacity;
    size_t size;
};

struct probe_result {
    size_t index;
    bool found;
};

static uint64_t hash_key(const char *key) {
    const unsigned char *byte = (const unsigned char *)key;
    uint64_t hash = UINT64_C(14695981039346656037);

    while (*byte != '\0') {
        hash ^= (uint64_t)*byte;
        hash *= UINT64_C(1099511628211);
        ++byte;
    }
    return hash;
}

/* Capacity is always a power of two. Linear probing checks string contents
 * after a hash match so collisions cannot merge distinct keys. */
static struct probe_result find_slot(
    const struct string_counter_entry *entries, size_t capacity,
    const char *key, uint64_t hash) {
    size_t index = (size_t)hash & (capacity - 1);

    for (size_t examined = 0; examined < capacity; ++examined) {
        const struct string_counter_entry *entry = &entries[index];
        if (entry->key == NULL) {
            return (struct probe_result){index, false};
        }
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            return (struct probe_result){index, true};
        }
        index = (index + 1) & (capacity - 1);
    }

    return (struct probe_result){0, false};
}

static size_t maximum_size(size_t capacity) {
    /* floor(0.7 * capacity), written without multiplying capacity by 7. */
    return (capacity / 10) * 7 + ((capacity % 10) * 7) / 10;
}

static bool resize_counter(struct string_counter *counter,
                           size_t new_capacity) {
    if (new_capacity > SIZE_MAX / sizeof(*counter->entries)) {
        return false;
    }

    struct string_counter_entry *new_entries =
        calloc(new_capacity, sizeof(*new_entries));
    if (new_entries == NULL) {
        return false;
    }

    for (size_t i = 0; i < counter->capacity; ++i) {
        struct string_counter_entry entry = counter->entries[i];
        if (entry.key == NULL) {
            continue;
        }

        struct probe_result slot =
            find_slot(new_entries, new_capacity, entry.key, entry.hash);
        if (slot.found || new_entries[slot.index].key != NULL) {
            /* The new table is larger and every old key is unique. This is a
             * defensive guard against publishing an invalid table. */
            free(new_entries);
            return false;
        }
        new_entries[slot.index] = entry;
    }

    free(counter->entries);
    counter->entries = new_entries;
    counter->capacity = new_capacity;
    return true;
}

static char *copy_key(const char *key) {
    size_t length = strlen(key);
    if (length == SIZE_MAX) {
        return NULL;
    }

    char *copy = malloc(length + 1);
    if (copy != NULL) {
        memcpy(copy, key, length + 1);
    }
    return copy;
}

struct string_counter *string_counter_create(void) {
    struct string_counter *counter = malloc(sizeof(*counter));
    if (counter == NULL) {
        return NULL;
    }

    counter->entries = calloc(INITIAL_CAPACITY, sizeof(*counter->entries));
    if (counter->entries == NULL) {
        free(counter);
        return NULL;
    }
    counter->capacity = INITIAL_CAPACITY;
    counter->size = 0;
    return counter;
}

void string_counter_destroy(struct string_counter *counter) {
    if (counter == NULL) {
        return;
    }

    for (size_t i = 0; i < counter->capacity; ++i) {
        free(counter->entries[i].key);
    }
    free(counter->entries);
    free(counter);
}

bool string_counter_increment(struct string_counter *counter, const char *key,
                              size_t *new_count) {
    if (counter == NULL || key == NULL || new_count == NULL) {
        return false;
    }

    uint64_t hash = hash_key(key);
    struct probe_result slot =
        find_slot(counter->entries, counter->capacity, key, hash);
    if (slot.found) {
        struct string_counter_entry *entry = &counter->entries[slot.index];
        if (entry->count == SIZE_MAX) {
            return false;
        }
        ++entry->count;
        *new_count = entry->count;
        return true;
    }

    if (counter->size == SIZE_MAX) {
        return false;
    }

    char *key_copy = copy_key(key);
    if (key_copy == NULL) {
        return false;
    }

    size_t new_size = counter->size + 1;
    if (new_size > maximum_size(counter->capacity)) {
        if (counter->capacity > SIZE_MAX / 2) {
            free(key_copy);
            return false;
        }
        size_t new_capacity = counter->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(*counter->entries) ||
            !resize_counter(counter, new_capacity)) {
            free(key_copy);
            return false;
        }
        slot = find_slot(counter->entries, counter->capacity, key, hash);
    }

    if (slot.found || counter->entries[slot.index].key != NULL) {
        free(key_copy);
        return false;
    }
    counter->entries[slot.index] =
        (struct string_counter_entry){key_copy, hash, 1};
    counter->size = new_size;
    *new_count = 1;
    return true;
}

bool string_counter_get(const struct string_counter *counter, const char *key,
                        size_t *count) {
    if (counter == NULL || key == NULL || count == NULL) {
        return false;
    }

    uint64_t hash = hash_key(key);
    struct probe_result slot =
        find_slot(counter->entries, counter->capacity, key, hash);
    if (!slot.found) {
        return false;
    }

    *count = counter->entries[slot.index].count;
    return true;
}

size_t string_counter_size(const struct string_counter *counter) {
    return counter == NULL ? 0 : counter->size;
}

bool string_counter_visit(const struct string_counter *counter,
                          string_counter_visit_fn visitor, void *context) {
    if (counter == NULL || visitor == NULL) {
        return false;
    }
    for (size_t index = 0; index < counter->capacity; ++index) {
        const struct string_counter_entry *entry = &counter->entries[index];
        if (entry->key != NULL && !visitor(entry->key, entry->count, context)) {
            return false;
        }
    }
    return true;
}
