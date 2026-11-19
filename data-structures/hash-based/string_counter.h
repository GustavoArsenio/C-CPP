#ifndef C_CPP_STRING_COUNTER_H
#define C_CPP_STRING_COUNTER_H

#include <stdbool.h>
#include <stddef.h>

struct string_counter;
typedef bool (*string_counter_visit_fn)(const char *key, size_t count,
                                        void *context);

/* Create an empty counter. Returns NULL when the required storage cannot be
 * allocated. Creation takes O(1) time and space. */
struct string_counter *string_counter_create(void);

/* Destroy a counter and every key copied into it. Accepts NULL. Time is
 * O(capacity + total key bytes). */
void string_counter_destroy(struct string_counter *counter);

/*
 * Add one occurrence of key. The counter copies key, so the caller retains
 * ownership of its buffer. Keys are NUL-terminated byte strings; comparison
 * is case-sensitive and an empty key is valid. On success, write the updated
 * count to new_count. On failure, return false without changing the counter
 * or new_count. Failure includes invalid arguments, allocation failure, or
 * count overflow.
 *
 * Expected time is O(1); worst-case time is O(n) for collisions or a resize.
 */
bool string_counter_increment(struct string_counter *counter, const char *key,
                              size_t *new_count);

/*
 * Look up key and write its count when found. Return false for invalid
 * arguments or a missing key, leaving count unchanged. Expected time is O(1),
 * worst-case O(n).
 */
bool string_counter_get(const struct string_counter *counter, const char *key,
                        size_t *count);

/* Return the number of distinct keys, or zero for NULL, in O(1) time. */
size_t string_counter_size(const struct string_counter *counter);

/* Visit each pair in unspecified table order in O(capacity) time. The callback
 * must not mutate the counter. Returns false for invalid arguments or if the
 * callback returns false; no key is transferred or writable through this
 * interface. */
bool string_counter_visit(const struct string_counter *counter,
                          string_counter_visit_fn visitor, void *context);

#endif
