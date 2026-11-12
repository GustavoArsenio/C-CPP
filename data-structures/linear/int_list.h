#ifndef C_CPP_INT_LIST_H
#define C_CPP_INT_LIST_H

#include <stdbool.h>
#include <stddef.h>

struct int_list;
typedef void (*int_list_visit_fn)(int value, void *context);

struct int_list *int_list_create(void);
bool int_list_append(struct int_list *list, int value);
size_t int_list_size(const struct int_list *list);
size_t int_list_length(const struct int_list *list);
size_t int_list_count_greater(const struct int_list *list, int value);
bool int_list_last(const struct int_list *list, int *value);
bool int_list_get_at(const struct int_list *list, size_t index, int *value);
void int_list_visit(const struct int_list *list, int_list_visit_fn visitor,
                    void *context);
size_t int_list_remove_all(struct int_list *list, int value);
struct int_list *int_list_split_after(struct int_list *list, int value);
void int_list_reverse(struct int_list *list);
void int_list_destroy(struct int_list *list);

#endif
