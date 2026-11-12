#ifndef C_CPP_LISTA4_LISTS_H
#define C_CPP_LISTA4_LISTS_H

#include <stdbool.h>
#include <stddef.h>

struct char_list;
struct circular_int_list;
struct doubly_int_list;
struct circular_doubly_int_list;
typedef void (*lista4_char_visit_fn)(char value, void *context);
typedef void (*lista4_int_visit_fn)(int value, void *context);

struct char_list *char_list_create(void);
bool char_list_append(struct char_list *list, char value);
size_t char_list_size(const struct char_list *list);
void char_list_visit(const struct char_list *list, lista4_char_visit_fn visitor,
                     void *context);
struct char_list *char_list_copy_iterative(const struct char_list *source);
struct char_list *char_list_copy_recursive(const struct char_list *source);
void char_list_destroy(struct char_list *list);

struct circular_int_list *circular_int_list_create(void);
bool circular_int_list_append(struct circular_int_list *list, int value);
bool circular_int_list_make_circular(struct circular_int_list *list);
bool circular_int_list_is_circular(const struct circular_int_list *list);
size_t circular_int_list_size(const struct circular_int_list *list);
void circular_int_list_visit(const struct circular_int_list *list,
                             lista4_int_visit_fn visitor, void *context);
void circular_int_list_destroy(struct circular_int_list *list);

struct doubly_int_list *doubly_int_list_create(void);
bool doubly_int_list_append(struct doubly_int_list *list, int value);
bool doubly_int_list_remove_first(struct doubly_int_list *list, int *value);
bool doubly_int_list_remove_last(struct doubly_int_list *list, int *value);
size_t doubly_int_list_size(const struct doubly_int_list *list);
void doubly_int_list_visit(const struct doubly_int_list *list,
                           lista4_int_visit_fn visitor, void *context);
void doubly_int_list_destroy(struct doubly_int_list *list);

struct circular_doubly_int_list *circular_doubly_int_list_create(void);
bool circular_doubly_int_list_insert(struct circular_doubly_int_list *list,
                                     size_t index, int value);
bool circular_doubly_int_list_remove(struct circular_doubly_int_list *list,
                                     size_t index, int *value);
size_t circular_doubly_int_list_size(
    const struct circular_doubly_int_list *list);
void circular_doubly_int_list_visit(const struct circular_doubly_int_list *list,
                                    lista4_int_visit_fn visitor, void *context);
void circular_doubly_int_list_visit_reverse(
    const struct circular_doubly_int_list *list, lista4_int_visit_fn visitor,
    void *context);
void circular_doubly_int_list_destroy(struct circular_doubly_int_list *list);

#endif
