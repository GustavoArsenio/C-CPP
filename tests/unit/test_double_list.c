#include <assert.h>

#include "double_list.h"

static void assert_values(const struct double_list *list,
                          const double *expected, size_t count) {
  assert(double_list_size(list) == count);
  for (size_t index = 0; index < count; ++index) {
    double actual;
    assert(double_list_get_at(list, index, &actual));
    assert(actual == expected[index]);
  }
  double ignored;
  assert(!double_list_get_at(list, count, &ignored));
}

static void append_values(struct double_list *list, const double *values,
                          size_t count) {
  for (size_t index = 0; index < count; ++index) {
    assert(double_list_append(list, values[index]));
  }
}

static void test_concatenate_move(void) {
  const double destination_values[] = {1.25, -2.5};
  const double source_values[] = {3.75, 4.5};
  const double expected[] = {1.25, -2.5, 3.75, 4.5};
  struct double_list *destination = double_list_create();
  struct double_list *source = double_list_create();
  assert(destination != NULL && source != NULL);
  append_values(destination, destination_values, 2);
  append_values(source, source_values, 2);
  assert(double_list_concat_move(destination, source));
  assert_values(destination, expected, 4);
  assert_values(source, NULL, 0);
  assert(double_list_concat_move(destination, source));
  assert_values(destination, expected, 4);
  assert(!double_list_concat_move(destination, destination));
  assert(!double_list_concat_move(NULL, source));
  assert(!double_list_concat_move(destination, NULL));
  double_list_destroy(destination);
  double_list_destroy(source);

  destination = double_list_create();
  source = double_list_create();
  assert(destination != NULL && source != NULL);
  append_values(source, source_values, 2);
  assert(double_list_concat_move(destination, source));
  assert_values(destination, source_values, 2);
  assert_values(source, NULL, 0);
  double_list_destroy(destination);
  double_list_destroy(source);
}

static void test_interleave_move(void) {
  const double first_values[] = {1, 3, 5};
  const double second_values[] = {2, 4};
  const double expected[] = {1, 2, 3, 4, 5};
  struct double_list *first = double_list_create();
  struct double_list *second = double_list_create();
  assert(first != NULL && second != NULL);
  append_values(first, first_values, 3);
  append_values(second, second_values, 2);
  struct double_list *merged = double_list_interleave_move(first, second);
  assert(merged != NULL);
  assert_values(merged, expected, 5);
  assert_values(first, NULL, 0);
  assert_values(second, NULL, 0);
  assert(double_list_interleave_move(first, first) == NULL);
  assert(double_list_interleave_move(NULL, second) == NULL);
  double_list_destroy(first);
  double_list_destroy(second);
  double_list_destroy(merged);

  first = double_list_create();
  second = double_list_create();
  assert(first != NULL && second != NULL);
  append_values(first, first_values, 1);
  append_values(second, second_values, 2);
  const double second_longer[] = {1, 2, 4};
  merged = double_list_interleave_move(first, second);
  assert(merged != NULL);
  assert_values(merged, second_longer, 3);
  assert_values(first, NULL, 0);
  assert_values(second, NULL, 0);
  double_list_destroy(first);
  double_list_destroy(second);
  double_list_destroy(merged);

  first = double_list_create();
  second = double_list_create();
  assert(first != NULL && second != NULL);
  append_values(second, second_values, 2);
  merged = double_list_interleave_move(first, second);
  assert(merged != NULL);
  assert_values(merged, second_values, 2);
  assert_values(first, NULL, 0);
  assert_values(second, NULL, 0);
  double_list_destroy(first);
  double_list_destroy(second);
  double_list_destroy(merged);

  first = double_list_create();
  second = double_list_create();
  assert(first != NULL && second != NULL);
  merged = double_list_interleave_move(first, second);
  assert(merged != NULL);
  assert_values(merged, NULL, 0);
  double_list_destroy(first);
  double_list_destroy(second);
  double_list_destroy(merged);
}

int main(void) {
  assert(double_list_size(NULL) == 0);
  assert(!double_list_append(NULL, 1.0));
  assert(!double_list_get_at(NULL, 0, NULL));
  double_list_visit(NULL, NULL, NULL);
  struct double_list *list = double_list_create();
  assert(list != NULL);
  assert(double_list_append(list, 1.5));
  assert(!double_list_get_at(list, 0, NULL));
  double_list_destroy(list);
  test_concatenate_move();
  test_interleave_move();
  double_list_destroy(NULL);
  return 0;
}
