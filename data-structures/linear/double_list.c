#include "double_list.h"

#include <stdlib.h>

struct double_list_node {
  double value;
  struct double_list_node *next;
};

struct double_list {
  struct double_list_node *head;
  struct double_list_node *tail;
  size_t size;
};

static void append_node(struct double_list *list,
                        struct double_list_node *node) {
  if (list == NULL || node == NULL) {
    return;
  }
  node->next = NULL;
  if (list->tail == NULL) {
    list->head = node;
  } else {
    list->tail->next = node;
  }
  list->tail = node;
  ++list->size;
}

static struct double_list_node *take_front(struct double_list *list) {
  struct double_list_node *node = list->head;
  if (node == NULL) {
    return NULL;
  }
  list->head = node->next;
  if (list->head == NULL) {
    list->tail = NULL;
  }
  --list->size;
  node->next = NULL;
  return node;
}

struct double_list *double_list_create(void) {
  return calloc(1, sizeof(struct double_list));
}

void double_list_destroy(struct double_list *list) {
  if (list == NULL) {
    return;
  }
  struct double_list_node *node = list->head;
  while (node != NULL) {
    struct double_list_node *next = node->next;
    free(node);
    node = next;
  }
  free(list);
}

size_t double_list_size(const struct double_list *list) {
  return list == NULL ? 0 : list->size;
}

bool double_list_append(struct double_list *list, double value) {
  if (list == NULL) {
    return false;
  }
  struct double_list_node *node = malloc(sizeof(*node));
  if (node == NULL) {
    return false;
  }
  node->value = value;
  append_node(list, node);
  return true;
}

bool double_list_get_at(const struct double_list *list, size_t index,
                        double *value) {
  if (list == NULL || value == NULL || index >= list->size) {
    return false;
  }
  const struct double_list_node *node = list->head;
  for (size_t current = 0; current < index; ++current) {
    node = node->next;
  }
  *value = node->value;
  return true;
}

void double_list_visit(const struct double_list *list,
                       double_list_visit_fn visitor, void *context) {
  if (list == NULL || visitor == NULL) {
    return;
  }
  for (const struct double_list_node *node = list->head; node != NULL;
       node = node->next) {
    visitor(node->value, context);
  }
}

bool double_list_concat_move(struct double_list *destination,
                             struct double_list *source) {
  if (destination == NULL || source == NULL || destination == source) {
    return false;
  }
  if (source->head == NULL) {
    return true;
  }
  if (destination->tail == NULL) {
    destination->head = source->head;
  } else {
    destination->tail->next = source->head;
  }
  destination->tail = source->tail;
  destination->size += source->size;
  source->head = NULL;
  source->tail = NULL;
  source->size = 0;
  return true;
}

struct double_list *double_list_interleave_move(struct double_list *first,
                                                struct double_list *second) {
  if (first == NULL || second == NULL || first == second) {
    return NULL;
  }
  struct double_list *result = double_list_create();
  if (result == NULL) {
    return NULL;
  }
  while (first->head != NULL && second->head != NULL) {
    append_node(result, take_front(first));
    append_node(result, take_front(second));
  }
  while (first->head != NULL) {
    append_node(result, take_front(first));
  }
  while (second->head != NULL) {
    append_node(result, take_front(second));
  }
  return result;
}
