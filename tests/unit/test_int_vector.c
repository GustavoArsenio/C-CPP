#include <assert.h>
#include <stddef.h>

#include "int_vector.h"

int main(void) {
    assert(int_vector_size(NULL) == 0);
    assert(!int_vector_push_back(NULL, 7));
    assert(!int_vector_pop_back(NULL, &(int){0}));
    assert(!int_vector_get(NULL, 0, &(int){0}));

    int_vector *values = int_vector_create();
    assert(values != NULL);
    assert(int_vector_size(values) == 0);

    int popped = -1;
    assert(!int_vector_pop_back(values, &popped));
    assert(!int_vector_pop_back(values, NULL));
    assert(!int_vector_get(values, 0, &popped));
    assert(!int_vector_get(values, 0, NULL));

    for (int value = 0; value < 33; ++value) {
        assert(int_vector_push_back(values, value * 3));
        assert(int_vector_size(values) == (size_t)value + 1);
    }
    for (size_t index = 0; index < int_vector_size(values); ++index) {
        int value = -1;
        assert(int_vector_get(values, index, &value));
        assert(value == (int)(index * 3));
    }
    assert(!int_vector_get(values, 33, &popped));

    for (int value = 32; value >= 0; --value) {
        assert(int_vector_pop_back(values, &popped));
        assert(popped == value * 3);
        assert(int_vector_size(values) == (size_t)value);
    }
    assert(!int_vector_pop_back(values, &popped));
    int_vector_destroy(values);
    int_vector_destroy(NULL);
    return 0;
}
