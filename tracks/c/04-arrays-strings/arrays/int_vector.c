#include "int_vector.h"

#include <stdint.h>
#include <stdlib.h>

struct int_vector {
    int *values;
    size_t size;
    size_t capacity;
};

int_vector *int_vector_create(void) {
    return calloc(1, sizeof(int_vector));
}

void int_vector_destroy(int_vector *vector) {
    if (vector != NULL) {
        free(vector->values);
        free(vector);
    }
}

static bool int_vector_grow(int_vector *vector) {
    size_t new_capacity = vector->capacity == 0 ? 4 : vector->capacity;
    if (vector->capacity != 0) {
        if (vector->capacity > SIZE_MAX / 2) {
            return false;
        }
        new_capacity *= 2;
    }
    if (new_capacity > SIZE_MAX / sizeof *vector->values) {
        return false;
    }

    int *new_values =
        realloc(vector->values, new_capacity * sizeof *vector->values);
    if (new_values == NULL) {
        return false;
    }
    vector->values = new_values;
    vector->capacity = new_capacity;
    return true;
}

bool int_vector_push_back(int_vector *vector, int value) {
    if (vector == NULL) {
        return false;
    }
    if (vector->size == vector->capacity && !int_vector_grow(vector)) {
        return false;
    }
    vector->values[vector->size] = value;
    ++vector->size;
    return true;
}

bool int_vector_pop_back(int_vector *vector, int *value) {
    if (vector == NULL || value == NULL || vector->size == 0) {
        return false;
    }
    --vector->size;
    *value = vector->values[vector->size];
    return true;
}

size_t int_vector_size(const int_vector *vector) {
    return vector == NULL ? 0 : vector->size;
}

bool int_vector_get(const int_vector *vector, size_t index, int *value) {
    if (vector == NULL || value == NULL || index >= vector->size) {
        return false;
    }
    *value = vector->values[index];
    return true;
}
