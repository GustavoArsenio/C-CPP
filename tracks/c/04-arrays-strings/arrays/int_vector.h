#ifndef INT_VECTOR_H
#define INT_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct int_vector int_vector;

/* The caller owns the returned vector and must destroy it. */
int_vector *int_vector_create(void);
void int_vector_destroy(int_vector *vector);

/* Append returns false without changing the vector if growth fails. */
bool int_vector_push_back(int_vector *vector, int value);
bool int_vector_pop_back(int_vector *vector, int *value);
size_t int_vector_size(const int_vector *vector);
bool int_vector_get(const int_vector *vector, size_t index, int *value);

#endif
