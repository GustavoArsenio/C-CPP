#include <stdio.h>

#include "data-structures/linear/int_list.h"

int main(void) {
    struct int_list *list = int_list_create();
    if (list == NULL) {
        return 1;
    }
    size_t count;
    if (scanf("%zu", &count) != 1) {
        int_list_destroy(list);
        return 1;
    }
    int value;
    for (size_t i = 0; i < count; ++i) {
        if (scanf("%d", &value) != 1) {
            int_list_destroy(list);
            return 1;
        }
        if (!int_list_append(list, value)) {
            int_list_destroy(list);
            return 1;
        }
    }
    int threshold;
    if (scanf("%d", &threshold) != 1) {
        int_list_destroy(list);
        return 1;
    }
    printf("%zu\n", int_list_count_greater(list, threshold));
    int_list_destroy(list);
    return 0;
}
