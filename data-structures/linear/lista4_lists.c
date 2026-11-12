#include "lista4_lists.h"

#include <stdlib.h>

struct char_node {
    char value;
    struct char_node *next;
};

struct char_list {
    struct char_node *head;
    struct char_node *tail;
    size_t size;
};

struct int_node {
    int value;
    struct int_node *next;
    struct int_node *previous;
};

struct circular_int_list {
    struct int_node *head;
    struct int_node *tail;
    size_t size;
    bool circular;
};

struct doubly_int_list {
    struct int_node *head;
    struct int_node *tail;
    size_t size;
};

struct circular_doubly_int_list {
    struct int_node *head;
    size_t size;
};

static struct int_node *new_int_node(int value) {
    struct int_node *node = malloc(sizeof(*node));
    if (node != NULL) {
        node->value = value;
        node->next = NULL;
        node->previous = NULL;
    }
    return node;
}

struct char_list *char_list_create(void) {
    return calloc(1, sizeof(struct char_list));
}

bool char_list_append(struct char_list *list, char value) {
    if (list == NULL || list->size == (size_t)-1) {
        return false;
    }
    struct char_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }
    node->value = value;
    node->next = NULL;
    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
    ++list->size;
    return true;
}

size_t char_list_size(const struct char_list *list) {
    return list == NULL ? 0 : list->size;
}

void char_list_visit(const struct char_list *list, lista4_char_visit_fn visitor,
                     void *context) {
    if (list == NULL || visitor == NULL) {
        return;
    }
    for (const struct char_node *node = list->head; node != NULL;
         node = node->next) {
        visitor(node->value, context);
    }
}

static struct char_list *copy_recursive_nodes(const struct char_node *source,
                                              size_t depth) {
    if (source == NULL) {
        return char_list_create();
    }
    if (depth == (size_t)-1) {
        return NULL;
    }
    struct char_list *copy = copy_recursive_nodes(source->next, depth + 1);
    if (copy == NULL) {
        return NULL;
    }
    struct char_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        char_list_destroy(copy);
        return NULL;
    }
    node->value = source->value;
    node->next = copy->head;
    copy->head = node;
    if (copy->tail == NULL) {
        copy->tail = node;
    }
    ++copy->size;
    return copy;
}

struct char_list *char_list_copy_iterative(const struct char_list *source) {
    if (source == NULL) {
        return NULL;
    }
    struct char_list *copy = char_list_create();
    if (copy == NULL) {
        return NULL;
    }
    for (const struct char_node *node = source->head; node != NULL;
         node = node->next) {
        if (!char_list_append(copy, node->value)) {
            char_list_destroy(copy);
            return NULL;
        }
    }
    return copy;
}

struct char_list *char_list_copy_recursive(const struct char_list *source) {
    if (source == NULL) {
        return NULL;
    }
    return copy_recursive_nodes(source->head, 0);
}

void char_list_destroy(struct char_list *list) {
    if (list == NULL) {
        return;
    }
    struct char_node *node = list->head;
    while (node != NULL) {
        struct char_node *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

struct circular_int_list *circular_int_list_create(void) {
    return calloc(1, sizeof(struct circular_int_list));
}

bool circular_int_list_append(struct circular_int_list *list, int value) {
    if (list == NULL || list->circular || list->size == (size_t)-1) {
        return false;
    }
    struct int_node *node = new_int_node(value);
    if (node == NULL) {
        return false;
    }
    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
    ++list->size;
    return true;
}

bool circular_int_list_make_circular(struct circular_int_list *list) {
    if (list == NULL) {
        return false;
    }
    if (!list->circular && list->tail != NULL) {
        list->tail->next = list->head;
    }
    list->circular = true;
    return true;
}

bool circular_int_list_is_circular(const struct circular_int_list *list) {
    if (list == NULL || !list->circular) {
        return false;
    }
    return list->size == 0 ||
           (list->tail != NULL && list->tail->next == list->head);
}

size_t circular_int_list_size(const struct circular_int_list *list) {
    return list == NULL ? 0 : list->size;
}

void circular_int_list_visit(const struct circular_int_list *list,
                             lista4_int_visit_fn visitor, void *context) {
    if (list == NULL || visitor == NULL) {
        return;
    }
    const struct int_node *node = list->head;
    for (size_t i = 0; i < list->size; ++i) {
        visitor(node->value, context);
        node = node->next;
    }
}

void circular_int_list_destroy(struct circular_int_list *list) {
    if (list == NULL) {
        return;
    }
    struct int_node *node = list->head;
    for (size_t i = 0; i < list->size; ++i) {
        struct int_node *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

struct doubly_int_list *doubly_int_list_create(void) {
    return calloc(1, sizeof(struct doubly_int_list));
}

bool doubly_int_list_append(struct doubly_int_list *list, int value) {
    if (list == NULL || list->size == (size_t)-1) {
        return false;
    }
    struct int_node *node = new_int_node(value);
    if (node == NULL) {
        return false;
    }
    node->previous = list->tail;
    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
    ++list->size;
    return true;
}

static bool doubly_remove_node(struct doubly_int_list *list,
                               struct int_node *node, int *value) {
    if (list == NULL || node == NULL || value == NULL) {
        return false;
    }
    *value = node->value;
    if (node->previous == NULL) {
        list->head = node->next;
    } else {
        node->previous->next = node->next;
    }
    if (node->next == NULL) {
        list->tail = node->previous;
    } else {
        node->next->previous = node->previous;
    }
    --list->size;
    free(node);
    return true;
}

bool doubly_int_list_remove_first(struct doubly_int_list *list, int *value) {
    return list != NULL && doubly_remove_node(list, list->head, value);
}

bool doubly_int_list_remove_last(struct doubly_int_list *list, int *value) {
    return list != NULL && doubly_remove_node(list, list->tail, value);
}

size_t doubly_int_list_size(const struct doubly_int_list *list) {
    return list == NULL ? 0 : list->size;
}

void doubly_int_list_visit(const struct doubly_int_list *list,
                           lista4_int_visit_fn visitor, void *context) {
    if (list == NULL || visitor == NULL) {
        return;
    }
    for (const struct int_node *node = list->head; node != NULL;
         node = node->next) {
        visitor(node->value, context);
    }
}

void doubly_int_list_destroy(struct doubly_int_list *list) {
    if (list == NULL) {
        return;
    }
    struct int_node *node = list->head;
    while (node != NULL) {
        struct int_node *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

struct circular_doubly_int_list *circular_doubly_int_list_create(void) {
    return calloc(1, sizeof(struct circular_doubly_int_list));
}

static struct int_node *circular_doubly_node_at(
    const struct circular_doubly_int_list *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }
    struct int_node *node = list->head;
    if (index <= list->size / 2) {
        for (size_t i = 0; i < index; ++i) {
            node = node->next;
        }
    } else {
        for (size_t i = list->size; i > index; --i) {
            node = node->previous;
        }
    }
    return node;
}

bool circular_doubly_int_list_insert(struct circular_doubly_int_list *list,
                                     size_t index, int value) {
    if (list == NULL || index > list->size || list->size == (size_t)-1) {
        return false;
    }
    struct int_node *node = new_int_node(value);
    if (node == NULL) {
        return false;
    }
    if (list->size == 0) {
        node->next = node;
        node->previous = node;
        list->head = node;
    } else {
        struct int_node *next = index == list->size
                                    ? list->head
                                    : circular_doubly_node_at(list, index);
        if (next == NULL) {
            free(node);
            return false;
        }
        struct int_node *previous = next->previous;
        node->previous = previous;
        node->next = next;
        previous->next = node;
        next->previous = node;
        if (index == 0) {
            list->head = node;
        }
    }
    ++list->size;
    return true;
}

bool circular_doubly_int_list_remove(struct circular_doubly_int_list *list,
                                     size_t index, int *value) {
    struct int_node *node = circular_doubly_node_at(list, index);
    if (node == NULL || value == NULL) {
        return false;
    }
    *value = node->value;
    if (list->size == 1) {
        list->head = NULL;
    } else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
        if (index == 0) {
            list->head = node->next;
        }
    }
    --list->size;
    free(node);
    return true;
}

size_t circular_doubly_int_list_size(
    const struct circular_doubly_int_list *list) {
    return list == NULL ? 0 : list->size;
}

void circular_doubly_int_list_visit(const struct circular_doubly_int_list *list,
                                    lista4_int_visit_fn visitor,
                                    void *context) {
    if (list == NULL || visitor == NULL) {
        return;
    }
    const struct int_node *node = list->head;
    for (size_t i = 0; i < list->size; ++i) {
        visitor(node->value, context);
        node = node->next;
    }
}

void circular_doubly_int_list_visit_reverse(
    const struct circular_doubly_int_list *list, lista4_int_visit_fn visitor,
    void *context) {
    if (list == NULL || visitor == NULL) {
        return;
    }
    const struct int_node *node =
        list->head == NULL ? NULL : list->head->previous;
    if (list->size != 0 && node == NULL) {
        return;
    }
    for (size_t i = 0; i < list->size; ++i) {
        visitor(node->value, context);
        node = node->previous;
    }
}

void circular_doubly_int_list_destroy(struct circular_doubly_int_list *list) {
    if (list == NULL) {
        return;
    }
    struct int_node *node = list->head;
    for (size_t i = 0; i < list->size; ++i) {
        struct int_node *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}
