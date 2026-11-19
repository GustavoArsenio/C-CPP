#include <assert.h>

#include "data-structures/linear/int_list.h"

int main(void) {
    struct int_list *list = int_list_create();
    assert(list != NULL);
    assert(int_list_size(list) == 0);
    assert(int_list_length(list) == 0);
    assert(int_list_append(list, 3));
    assert(int_list_append(list, 17));
    assert(int_list_append(list, 5));
    assert(int_list_append(list, 17));
    assert(int_list_size(list) == 4);
    assert(int_list_length(list) == 4);
    assert(int_list_count_greater(list, 5) == 2);
    assert(int_list_count_greater(NULL, 0) == 0);
    int last = 0;
    assert(int_list_last(list, &last) && last == 17);
    assert(!int_list_last(list, NULL));
    assert(int_list_remove_all(list, 17) == 2);
    assert(int_list_size(list) == 2);
    assert(int_list_length(list) == 2);
    assert(int_list_last(list, &last) && last == 5);
    assert(int_list_remove_all(list, 3) == 1);
    assert(int_list_size(list) == 1);
    assert(int_list_append(list, 9));
    assert(int_list_last(list, &last) && last == 9);
    assert(int_list_get_at(list, 1, &last) && last == 9);
    assert(!int_list_get_at(list, 2, &last));
    assert(int_list_remove_all(list, 9) == 1);
    assert(int_list_last(list, &last) && last == 5);
    assert(int_list_remove_all(list, 5) == 1);
    assert(int_list_size(list) == 0);
    assert(int_list_remove_all(list, 5) == 0);
    assert(!int_list_last(list, &last));
    assert(int_list_append(list, 3));
    assert(int_list_append(list, 17));
    assert(int_list_append(list, 5));
    assert(int_list_append(list, 12));
    struct int_list *suffix = int_list_split_after(list, 17);
    assert(suffix != NULL);
    assert(int_list_size(list) == 2);
    assert(int_list_size(suffix) == 2);
    assert(int_list_get_at(suffix, 0, &last) && last == 5);
    assert(int_list_get_at(suffix, 1, &last) && last == 12);
    struct int_list *unmatched = int_list_split_after(list, 99);
    assert(unmatched != NULL);
    assert(int_list_size(unmatched) == 0);
    int_list_reverse(suffix);
    assert(int_list_get_at(suffix, 0, &last) && last == 12);
    assert(int_list_last(suffix, &last) && last == 5);
    int_list_reverse(NULL);
    int_list_destroy(unmatched);
    int_list_destroy(suffix);
    int_list_destroy(list);
    int_list_destroy(NULL);
    assert(!int_list_append(NULL, 1));
    return 0;
}
