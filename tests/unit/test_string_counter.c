#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data-structures/hash-based/string_counter.h"

#define CHECK(condition)                                                       \
    do {                                                                       \
        if (!(condition)) {                                                    \
            fprintf(stderr, "check failed at %s:%d: %s\n", __FILE__, __LINE__, \
                    #condition);                                               \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

static void expect_count(const struct string_counter *counter, const char *key,
                         size_t expected) {
    size_t actual = SIZE_MAX;
    CHECK(string_counter_get(counter, key, &actual));
    CHECK(actual == expected);
}

struct visit_state {
    size_t seen;
    size_t total;
};
static bool count_visit(const char *key, size_t count, void *context) {
    struct visit_state *state = context;
    CHECK(key != NULL);
    CHECK(count > 0);
    ++state->seen;
    state->total += count;
    return true;
}
static bool stop_visit(const char *key, size_t count, void *context) {
    (void)key;
    (void)count;
    (void)context;
    return false;
}

int main(void) {
    string_counter_destroy(NULL);
    CHECK(string_counter_size(NULL) == 0);
    CHECK(!string_counter_get(NULL, "missing", NULL));
    CHECK(!string_counter_increment(NULL, "missing", NULL));

    struct string_counter *counter = string_counter_create();
    CHECK(counter != NULL);
    CHECK(string_counter_size(counter) == 0);

    size_t output = 73;
    CHECK(!string_counter_get(counter, "missing", &output));
    CHECK(output == 73);
    CHECK(!string_counter_get(counter, NULL, &output));
    CHECK(output == 73);
    CHECK(!string_counter_get(counter, "missing", NULL));
    CHECK(!string_counter_increment(counter, NULL, &output));
    CHECK(output == 73);
    CHECK(!string_counter_increment(counter, "valid", NULL));
    CHECK(string_counter_size(counter) == 0);

    CHECK(string_counter_increment(counter, "", &output));
    CHECK(output == 1);
    CHECK(string_counter_increment(counter, "", &output));
    CHECK(output == 2);
    expect_count(counter, "", 2);

    char temporary_key[] = "original";
    CHECK(string_counter_increment(counter, temporary_key, &output));
    CHECK(output == 1);
    memcpy(temporary_key, "mutated!", sizeof(temporary_key));
    expect_count(counter, "original", 1);
    CHECK(!string_counter_get(counter, "mutated!", &output));
    CHECK(output == 1);

    CHECK(string_counter_increment(counter, "Word", &output));
    CHECK(output == 1);
    CHECK(string_counter_increment(counter, "word", &output));
    CHECK(output == 1);
    CHECK(string_counter_increment(counter, "word", &output));
    CHECK(output == 2);
    expect_count(counter, "Word", 1);
    expect_count(counter, "word", 2);

    /* These pairs share the low four bits of the FNV-1a hash. The second
     * pair starts at the final slot of the initial table and probes through
     * index zero, covering collision chains and wraparound. */
    CHECK(string_counter_increment(counter, "collision5", &output));
    CHECK(string_counter_increment(counter, "collision10", &output));
    CHECK(string_counter_increment(counter, "wrap0", &output));
    CHECK(string_counter_increment(counter, "wrap19", &output));
    expect_count(counter, "collision5", 1);
    expect_count(counter, "collision10", 1);
    expect_count(counter, "wrap0", 1);
    expect_count(counter, "wrap19", 1);

    enum { GENERATED_KEYS = 1500 };
    char key[32];
    for (size_t index = 0; index < GENERATED_KEYS; ++index) {
        int length = snprintf(key, sizeof(key), "item-%zu", index);
        CHECK(length > 0 && (size_t)length < sizeof(key));
        CHECK(string_counter_increment(counter, key, &output));
        CHECK(output == 1);
    }
    CHECK(string_counter_size(counter) == GENERATED_KEYS + 8);

    struct visit_state visited = {0, 0};
    CHECK(string_counter_visit(counter, count_visit, &visited));
    CHECK(visited.seen == string_counter_size(counter));
    CHECK(visited.total == GENERATED_KEYS + 10);
    CHECK(!string_counter_visit(counter, NULL, &visited));
    CHECK(!string_counter_visit(counter, stop_visit, NULL));

    for (size_t index = 0; index < GENERATED_KEYS; ++index) {
        int length = snprintf(key, sizeof(key), "item-%zu", index);
        CHECK(length > 0 && (size_t)length < sizeof(key));
        expect_count(counter, key, 1);
    }

    CHECK(string_counter_increment(counter, "item-17", &output));
    CHECK(output == 2);
    expect_count(counter, "item-17", 2);
    CHECK(string_counter_size(counter) == GENERATED_KEYS + 8);

    string_counter_destroy(counter);
    return EXIT_SUCCESS;
}
