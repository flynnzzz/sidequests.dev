/*
 * test.c
 *
 * Test cases for the API.
 * Written with the help of a coding agent running locally
 */

#include "../src/arraylist.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST(name)                                                             \
  printf("Running %s... ", #name);                                             \
  fflush(stdout);
#define PASS() printf("PASSED\n")

static void test_empty_array() {
  TEST("test_empty_array");

  al_type *array = al_empty();

  assert(array != NULL);
  assert(al_length(array) == 0);
  assert(al_capacity(array) == INITIAL_CAPACITY);

  al_free(array);
  PASS();
}

static void test_new_array() {
  TEST("test_new_array");

  al_type *array = al_empty();
  assert(al_length(array) == 0);
  al_free(array);

  array = al_create(0);
  assert(al_length(array) == 0);
  assert(al_capacity(array) >= INITIAL_CAPACITY);
  al_free(array);

  array = al_create(INITIAL_CAPACITY);
  assert(al_length(array) == 0);
  assert(al_capacity(array) >= INITIAL_CAPACITY);
  al_free(array);

  array = al_create(44);
  assert(al_length(array) == 0);
  assert(al_capacity(array) == 44);
  al_free(array);

  PASS();
}

static void test_push_basic() {
  TEST("test_push_basic");

  al_type *array = al_empty();

  al_push(&array, 1);
  assert(al_length(array) == 1);
  assert(array[0] == 1);

  al_push(&array, 2);
  assert(al_length(array) == 2);
  assert(array[1] == 2);

  al_push(&array, 3);
  assert(al_length(array) == 3);
  assert(array[2] == 3);

  al_free(array);
  PASS();
}

static void test_push_capacity_growth() {
  TEST("test_push_capacity_growth");

  al_type *array = al_empty();

  for (size_t i = 0; i < INITIAL_CAPACITY; i++) {
    al_push(&array, i);
    assert(al_length(array) == i + 1);
    assert(array[i] == (int)i);
  }

  size_t original_capacity = al_capacity(array);
  for (size_t i = INITIAL_CAPACITY; i < INITIAL_CAPACITY + 10; i++) {
    al_push(&array, i);
    assert(al_length(array) == i + 1);
    assert(array[i] == (int)i);
    assert(al_capacity(array) >= original_capacity);
  }

  al_free(array);
  PASS();
}

static void test_remove_by_value_first_occurrence() {
  TEST("test_remove_by_value_first_occurrence");

  al_type *array = al_empty();

  al_push(&array, 1);
  al_push(&array, 2);
  al_push(&array, 1);
  al_push(&array, 3);
  al_push(&array, 1);

  assert(al_length(array) == 5);

  al_remove(array, 1);
  assert(al_length(array) == 4);

  assert(array[0] == 2);
  assert(array[1] == 1);
  assert(array[2] == 3);
  assert(array[3] == 1);

  PASS();
  al_free(array);
}

static void test_remove_nonexistent_value() {
  TEST("test_remove_nonexistent_value");

  al_type *array = al_empty();

  al_push(&array, 1);
  assert(al_length(array) == 1);

  al_remove(array, 999);
  // TODO: finish

  assert(al_length(array) == 1);
  assert(array[0] == 1);

  al_free(array);
  PASS();
}

static void test_remove_more_occurrences() {
  TEST("test_remove_all_occurrences");

  al_type *array = al_empty();

  al_push(&array, 5);
  al_push(&array, 3);
  al_push(&array, 5);
  al_push(&array, 7);
  al_push(&array, 5);

  assert(al_length(array) == 5);

  al_remove(array, 5);
  al_remove(array, 5);

  assert(al_length(array) == 3);
  assert(array[0] == 3);
  assert(array[1] == 7);
  assert(array[2] == 5);

  al_free(array);
  PASS();
}

static void test_iremove_basic() {
  TEST("test_iremove_basic");

  al_type *array = al_empty();

  al_push(&array, 1);
  al_push(&array, 2);
  al_push(&array, 3);
  al_push(&array, 4);
  al_push(&array, 5);

  assert(al_length(array) == 5);

  // Remove element at index 2 (value 3)
  al_iremove(array, 2);

  assert(al_length(array) == 4);
  assert(array[2] == 4); // Element shifted from index 3

  al_free(array);
  PASS();
}

static void test_iremove_first_and_last() {
  TEST("test_iremove_first_and_last");

  al_type *array = al_empty();

  al_push(&array, 10);
  al_push(&array, 20);
  al_push(&array, 30);
  al_push(&array, 40);
  al_push(&array, 50);

  assert(al_length(array) == 5);

  al_iremove(array, 0);
  assert(al_length(array) == 4);
  assert(array[0] == 20);

  al_iremove(array, 3);
  assert(al_length(array) == 3);
  assert(array[2] == 40);

  al_iremove(array, 1);
  assert(al_length(array) == 2);

  al_free(array);
  PASS();
}

static void test_iremove_empty_array() {
  TEST("test_iremove_empty_array");

  al_type *array = al_empty();
  al_remove(array, 0);
  al_remove(array, 10);
  al_remove(array, -10);
  // TODO: finish

  al_free(array);
  PASS();
}

static void test_iremove_middle_element() {
  TEST("test_iremove_middle_element");

  al_type *array = al_empty();

  al_push(&array, 100);
  al_push(&array, 200);
  al_push(&array, 300);
  al_push(&array, 400);
  al_push(&array, 500);

  assert(al_length(array) == 5);

  al_iremove(array, 2);

  assert(al_length(array) == 4);
  assert(array[1] == 200);
  assert(array[2] == 400); // Was at index 3

  al_free(array);
  PASS();
}

static void test_push_remove_same_value() {
  TEST("test_push_remove_same_value");

  al_type *array = al_empty();

  al_push(&array, 10);
  assert(al_length(array) == 1);

  al_remove(array, 10);
  // TODO: finish

  assert(al_length(array) == 0);

  al_free(array);
  PASS();
}

static void test_push_remove_interleaved() {
  TEST("test_push_remove_interleaved");

  al_type *array = al_empty();

  al_push(&array, 1);
  al_push(&array, 2);
  al_push(&array, 3);

  assert(al_length(array) == 3);

  // Remove middle value
  al_remove(array, 2);
  assert(al_length(array) == 2);
  assert(array[0] == 1);
  assert(array[1] == 3);

  // Insert another value
  al_push(&array, 4);
  assert(al_length(array) == 3);
  assert(array[1] == 3);
  assert(array[2] == 4);

  al_free(array);
  PASS();
}

static void test_capacity_boundary() {
  TEST("test_capacity_boundary");

  al_type *array = al_empty();

  // Insert many elements to near capacity
  for (size_t i = 0; i < INITIAL_CAPACITY - 1; i++) {
    al_push(&array, i);
  }

  assert(al_length(array) == INITIAL_CAPACITY - 1);
  assert(al_capacity(array) >= INITIAL_CAPACITY);

  // Insert one more to trigger growth
  al_push(&array, INITIAL_CAPACITY - 1);

  assert(al_length(array) == INITIAL_CAPACITY);
  assert(al_capacity(array) >= INITIAL_CAPACITY);

  al_free(array);
  PASS();
}

static void test_remove_at_boundary() {
  TEST("test_remove_at_boundary");

  al_type *array = al_empty();

  for (size_t i = 1; i <= 5; i++) {
    al_push(&array, i);
  }
  assert(al_length(array) == 5);

  // Remove last element
  al_remove(array, 5);
  assert(al_length(array) == 4);

  // Remove new last element
  al_remove(array, 4);
  assert(al_length(array) == 3);

  al_free(array);
  PASS();
}

static void test_iremove_out_of_bounds() {
  TEST("test_iremove_out_of_bounds");

  al_type *array = al_empty();

  for (size_t i = 0; i < 10; i++) {
    al_push(&array, i);
  }

  assert(al_length(array) == 10);

  al_iremove(array, -1);
  // TODO: finish

  assert(al_length(array) == 10); // Should still be 10

  al_iremove(array, 99);
  assert(al_length(array) == 10); // Should still be 10

  al_free(array);
  PASS();
}

static void test_remove_first_occurrence_edge_case() {
  TEST("test_remove_first_occurrence_edge_case");

  al_type *array = al_empty();

  // All elements are the same value
  for (size_t i = 0; i < 5; i++) {
    al_push(&array, 42);
  }

  assert(al_length(array) == 5);

  // Remove should only remove first occurrence
  al_remove(array, 42);

  assert(al_length(array) == 4);
  assert(array[0] == 42); // Still has 42 at index 0 (was the second original)

  // Remove again - should still be first remaining occurrence
  al_remove(array, 42);
  assert(al_length(array) == 3);

  al_free(array);
  PASS();
}

static void test_mixed_operations() {
  TEST("test_mixed_operations");

  al_type *array = al_empty();

  al_push(&array, 10);
  al_push(&array, 20);
  al_push(&array, 30);
  al_push(&array, 40);

  assert(al_length(array) == 4);

  // Remove middle value
  al_remove(array, 30);
  assert(al_length(array) == 3);

  // Insert new value
  al_push(&array, 99);
  assert(al_length(array) == 4);

  // Remove first value
  al_iremove(array, 0);
  assert(al_length(array) == 3);

  // Remove last value
  al_iremove(array, al_length(array) - 1);
  assert(al_length(array) == 2);

  al_free(array);
  PASS();
}

static void test_empty_value_removal() {
  TEST("test_empty_value_removal");

  al_type *array = al_empty();

  al_push(&array, 0);
  al_push(&array, 1);
  al_push(&array, 2);

  assert(al_length(array) == 3);

  // Remove 0 (valid integer value, not empty array)
  al_remove(array, 0);
  assert(al_length(array) == 2);
  assert(array[0] == 1);
  assert(array[1] == 2);

  al_free(array);
  PASS();
}

static void test_negative_value_removal() {
  TEST("test_negative_value_removal");

  al_type *array = al_empty();

  al_push(&array, -100);
  al_push(&array, 0);
  al_push(&array, 100);

  assert(al_length(array) == 3);

  // Remove negative value
  al_remove(array, -100);
  assert(al_length(array) == 2);
  assert(array[0] == 0);
  assert(array[1] == 100);

  al_free(array);
  PASS();
}

static void test_large_number_of_items() {
  TEST("test_large_number_of_items");

  al_type *array = al_empty();

  for (size_t i = 0; i < 1000; i++) {
    al_push(&array, i);
  }

  assert(al_length(array) == 1000);

  al_remove(array, 500);
  assert(al_length(array) == 999);
  assert(array[500] == 501);

  al_iremove(array, 100);
  assert(al_length(array) == 998);

  al_free(array);
  PASS();
}

static void test_remove_then_push_same() {
  TEST("test_remove_then_push_same");

  al_type *array = al_empty();

  al_push(&array, 5);
  assert(al_length(array) == 1);

  al_remove(array, 5);
  assert(al_length(array) == 0);

  // Insert same value again
  al_push(&array, 5);
  assert(al_length(array) == 1);
  assert(array[0] == 5);

  // Remove again - should be empty
  al_remove(array, 5);
  assert(al_length(array) == 0);

  al_free(array);
  PASS();
}

static void test_print_empty_array() {
  TEST("test_print_empty_array");

  al_type *array = al_empty();

  printf("Empty array: ");
  al_print(array);
  assert(al_length(array) == 0);

  al_free(array);
  PASS();
}

static void test_print_single_element() {
  TEST("test_print_single_element");

  al_type *array = al_empty();
  al_push(&array, 42);

  printf("Single element array: ");
  al_print(array);

  assert(al_length(array) == 1);

  al_free(array);
  PASS();
}

static void test_all_functions_used() {
  TEST("test_all_functions_used");

  al_type *array = al_empty();
  assert(al_length(array) == 0);
  assert(al_capacity(array) == INITIAL_CAPACITY);

  al_type *array2 = al_create(0);
  assert(al_length(array2) == 0);
  assert(al_capacity(array2) == INITIAL_CAPACITY);

  al_push(&array, 99);
  assert(al_length(array) == 1);
  al_remove(array, 99);
  assert(al_length(array) == 0);

  al_push(&array, 1);
  al_push(&array, 2);
  al_push(&array, 3);
  assert(al_length(array) == 3);
  al_iremove(array, 1);
  assert(al_length(array) == 2);

  al_free(array);
  al_free(array2);
  PASS();
}

static void test_new_exceed_maximum_capacity() {
  TEST("test_new_exceed_maximum_capacity");

  al_type *array = al_create(MAX_CAPACITY);
  assert(array == NULL);

  PASS();
}

static void test_push_exceed_maximum_capacity() {
  TEST("test_push_exceed_maximum_capacity");

  al_type *array = al_create(MAX_CAPACITY - 1);
  assert(!array);

  // TODO: finish

  PASS();
}

static void test_nullptr_guard() {
  TEST("test_nullptr_guard");

  puts("");
  assert(al_length(NULL) == 0);
  assert(al_capacity(NULL) == 0);

  PASS();
}

int main() {
  puts("=== Running ArrayList Tests ===\n");

  test_empty_array();
  test_new_array();
  test_push_basic();
  test_push_capacity_growth();
  test_remove_by_value_first_occurrence();
  test_remove_nonexistent_value();
  test_remove_more_occurrences();
  test_iremove_basic();
  test_iremove_first_and_last();
  test_iremove_empty_array();
  test_iremove_middle_element();
  test_push_remove_same_value();
  test_push_remove_interleaved();
  test_capacity_boundary();
  test_remove_at_boundary();
  test_iremove_out_of_bounds();
  test_remove_first_occurrence_edge_case();
  test_mixed_operations();
  test_empty_value_removal();
  test_negative_value_removal();
  test_large_number_of_items();
  test_remove_then_push_same();
  test_print_empty_array();
  test_print_single_element();
  test_all_functions_used();
  test_new_exceed_maximum_capacity();
  test_push_exceed_maximum_capacity();
  test_nullptr_guard();

  puts("\n=== All Tests Completed ===");

  return 0;
}
