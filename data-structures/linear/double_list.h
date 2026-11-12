#ifndef C_CPP_DOUBLE_LIST_H
#define C_CPP_DOUBLE_LIST_H

#include <stdbool.h>
#include <stddef.h>

struct double_list;
typedef void (*double_list_visit_fn)(double value, void *context);

struct double_list *double_list_create(void);
void double_list_destroy(struct double_list *list);
size_t double_list_size(const struct double_list *list);
bool double_list_append(struct double_list *list, double value);
bool double_list_get_at(const struct double_list *list, size_t index,
                        double *value);
void double_list_visit(const struct double_list *list,
                       double_list_visit_fn visitor, void *context);
/* Relink all source nodes to destination and leave source empty. */
bool double_list_concat_move(struct double_list *destination,
                             struct double_list *source);
/* Interleave and relink source nodes; both inputs are empty on success. */
struct double_list *double_list_interleave_move(struct double_list *first,
                                                struct double_list *second);

#endif
