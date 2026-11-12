#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "lista4_lists.h"

struct values {
    int items[16];
    size_t count;
};

static void collect_int(int value, void *context) {
    struct values *values = context;
    assert(values->count < 16);
    values->items[values->count++] = value;
}

struct chars {
    char items[16];
    size_t count;
};

static void collect_char(char value, void *context) {
    struct chars *chars = context;
    assert(chars->count < 16);
    chars->items[chars->count++] = value;
}

static void test_character_copies(void) {
    struct char_list *source = char_list_create();
    assert(source != NULL);
    struct char_list *empty_iterative = char_list_copy_iterative(source);
    struct char_list *empty_recursive = char_list_copy_recursive(source);
    assert(empty_iterative != NULL && char_list_size(empty_iterative) == 0);
    assert(empty_recursive != NULL && char_list_size(empty_recursive) == 0);
    assert(char_list_copy_iterative(NULL) == NULL);
    assert(char_list_copy_recursive(NULL) == NULL);
    assert(char_list_append(source, 'a'));
    assert(char_list_append(source, '\0'));
    assert(char_list_append(source, 'z'));
    struct char_list *iterative = char_list_copy_iterative(source);
    struct char_list *recursive = char_list_copy_recursive(source);
    assert(iterative != NULL && recursive != NULL);
    assert(char_list_size(iterative) == 3 && char_list_size(recursive) == 3);
    struct chars original = {{0}, 0};
    struct chars first = {{0}, 0};
    struct chars second = {{0}, 0};
    char_list_visit(source, collect_char, &original);
    char_list_visit(iterative, collect_char, &first);
    char_list_visit(recursive, collect_char, &second);
    for (size_t i = 0; i < 3; ++i) {
        assert(original.items[i] == first.items[i]);
        assert(original.items[i] == second.items[i]);
    }
    char_list_destroy(source);
    struct chars after_source_destroy = {{0}, 0};
    char_list_visit(recursive, collect_char, &after_source_destroy);
    assert(after_source_destroy.count == 3);
    char_list_destroy(empty_iterative);
    char_list_destroy(empty_recursive);
    char_list_destroy(iterative);
    char_list_destroy(recursive);
}

static void test_singly_circular_conversion(void) {
    struct circular_int_list *list = circular_int_list_create();
    assert(list != NULL);
    assert(!circular_int_list_is_circular(list));
    assert(circular_int_list_make_circular(list));
    assert(circular_int_list_is_circular(list));
    assert(!circular_int_list_append(list, 8));
    circular_int_list_destroy(list);

    list = circular_int_list_create();
    assert(list != NULL);
    assert(circular_int_list_append(list, 7));
    assert(circular_int_list_append(list, 7));
    assert(circular_int_list_append(list, -2));
    assert(circular_int_list_make_circular(list));
    assert(circular_int_list_is_circular(list));
    assert(circular_int_list_size(list) == 3);
    struct values values = {{0}, 0};
    circular_int_list_visit(list, collect_int, &values);
    assert(values.count == 3);
    assert(values.items[0] == 7 && values.items[1] == 7 &&
           values.items[2] == -2);
    circular_int_list_destroy(list);
}

static void test_doubly_linked_end_removal(void) {
    struct doubly_int_list *list = doubly_int_list_create();
    int value;
    assert(list != NULL);
    assert(!doubly_int_list_remove_first(list, &value));
    assert(!doubly_int_list_remove_last(list, &value));
    assert(doubly_int_list_append(list, 4));
    assert(!doubly_int_list_remove_first(list, NULL));
    assert(doubly_int_list_remove_first(list, &value) && value == 4);
    assert(doubly_int_list_size(list) == 0);
    assert(doubly_int_list_append(list, 1));
    assert(doubly_int_list_append(list, 2));
    assert(doubly_int_list_append(list, 3));
    assert(doubly_int_list_remove_first(list, &value) && value == 1);
    assert(doubly_int_list_remove_last(list, &value) && value == 3);
    struct values values = {{0}, 0};
    doubly_int_list_visit(list, collect_int, &values);
    assert(values.count == 1 && values.items[0] == 2);
    assert(doubly_int_list_remove_last(list, &value) && value == 2);
    assert(doubly_int_list_size(list) == 0);
    doubly_int_list_destroy(list);
}

static void test_circular_doubly_list(void) {
    struct circular_doubly_int_list *list = circular_doubly_int_list_create();
    int value;
    assert(list != NULL);
    assert(!circular_doubly_int_list_insert(list, 1, 10));
    assert(!circular_doubly_int_list_remove(list, 0, &value));
    assert(circular_doubly_int_list_insert(list, 0, 2));
    assert(circular_doubly_int_list_insert(list, 1, 4));
    assert(circular_doubly_int_list_insert(list, 1, 3));
    assert(circular_doubly_int_list_insert(list, 0, 1));
    assert(!circular_doubly_int_list_insert(list, 5, 9));
    assert(circular_doubly_int_list_size(list) == 4);
    struct values forward = {{0}, 0};
    struct values backward = {{0}, 0};
    circular_doubly_int_list_visit(list, collect_int, &forward);
    circular_doubly_int_list_visit_reverse(list, collect_int, &backward);
    assert(forward.count == 4 && backward.count == 4);
    assert(forward.items[0] == 1 && forward.items[1] == 2 &&
           forward.items[2] == 3 && forward.items[3] == 4);
    assert(backward.items[0] == 4 && backward.items[1] == 3 &&
           backward.items[2] == 2 && backward.items[3] == 1);
    assert(!circular_doubly_int_list_remove(list, 2, NULL));
    assert(circular_doubly_int_list_remove(list, 0, &value) && value == 1);
    assert(circular_doubly_int_list_remove(list, 2, &value) && value == 4);
    assert(circular_doubly_int_list_remove(list, 1, &value) && value == 3);
    assert(circular_doubly_int_list_remove(list, 0, &value) && value == 2);
    assert(circular_doubly_int_list_size(list) == 0);
    assert(!circular_doubly_int_list_remove(list, 0, &value));
    circular_doubly_int_list_destroy(list);
}

int main(void) {
    test_character_copies();
    test_singly_circular_conversion();
    test_doubly_linked_end_removal();
    test_circular_doubly_list();
    return 0;
}
