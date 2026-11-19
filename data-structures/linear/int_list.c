#include "int_list.h"

#include <stdlib.h>

struct list_node {
    int value;
    struct list_node *next;
};

struct int_list {
    struct list_node *head;
    struct list_node *tail;
    size_t size;
};

struct int_list *int_list_create(void) {
    return calloc(1, sizeof(struct int_list));
}

bool int_list_append(struct int_list *list, int value) {
    if (list == NULL) {
        return false;
    }
    struct list_node *node = malloc(sizeof(*node));
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

size_t int_list_size(const struct int_list *list) {
    return list == NULL ? 0 : list->size;
}

size_t int_list_length(const struct int_list *list) {
    size_t length = 0;
    if (list == NULL) {
        return 0;
    }
    for (const struct list_node *node = list->head; node != NULL;
         node = node->next) {
        ++length;
    }
    return length;
}

size_t int_list_count_greater(const struct int_list *list, int value) {
    size_t count = 0;
    if (list == NULL) {
        return 0;
    }
    for (const struct list_node *node = list->head; node != NULL;
         node = node->next) {
        if (node->value > value) {
            ++count;
        }
    }
    return count;
}

bool int_list_last(const struct int_list *list, int *value) {
    if (list == NULL || value == NULL || list->head == NULL) {
        return false;
    }
    const struct list_node *node = list->head;
    while (node->next != NULL) {
        node = node->next;
    }
    *value = node->value;
    return true;
}

bool int_list_get_at(const struct int_list *list, size_t index, int *value) {
    if (list == NULL || value == NULL || index >= list->size) {
        return false;
    }
    const struct list_node *node = list->head;
    for (size_t current = 0; current < index; ++current) {
        node = node->next;
    }
    *value = node->value;
    return true;
}

void int_list_visit(const struct int_list *list, int_list_visit_fn visitor,
                    void *context) {
    if (list == NULL || visitor == NULL) {
        return;
    }
    for (const struct list_node *node = list->head; node != NULL;
         node = node->next) {
        visitor(node->value, context);
    }
}

size_t int_list_remove_all(struct int_list *list, int value) {
    if (list == NULL) {
        return 0;
    }
    size_t removed = 0;
    struct list_node **link = &list->head;
    while (*link != NULL) {
        struct list_node *node = *link;
        if (node->value == value) {
            *link = node->next;
            if (list->tail == node) {
                list->tail = NULL;
            }
            free(node);
            --list->size;
            ++removed;
        } else {
            link = &node->next;
        }
    }
    if (list->head == NULL) {
        list->tail = NULL;
    } else if (list->tail == NULL) {
        struct list_node *tail = list->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        list->tail = tail;
    }
    return removed;
}

struct int_list *int_list_split_after(struct int_list *list, int value) {
    if (list == NULL) {
        return NULL;
    }
    struct list_node *split = list->head;
    while (split != NULL && split->value != value) {
        split = split->next;
    }
    struct int_list *suffix = int_list_create();
    if (suffix == NULL) {
        return NULL;
    }
    if (split == NULL) {
        return suffix;
    }

    suffix->head = split->next;
    split->next = NULL;
    list->tail = split;
    for (struct list_node *node = suffix->head; node != NULL;
         node = node->next) {
        ++suffix->size;
    }
    suffix->tail = suffix->head;
    while (suffix->tail != NULL && suffix->tail->next != NULL) {
        suffix->tail = suffix->tail->next;
    }
    list->size -= suffix->size;
    return suffix;
}

void int_list_reverse(struct int_list *list) {
    if (list == NULL || list->head == NULL) {
        return;
    }
    struct list_node *previous = NULL;
    struct list_node *current = list->head;
    list->tail = list->head;
    while (current != NULL) {
        struct list_node *next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    list->head = previous;
}

void int_list_destroy(struct int_list *list) {
    if (list == NULL) {
        return;
    }
    struct list_node *node = list->head;
    while (node != NULL) {
        struct list_node *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}
