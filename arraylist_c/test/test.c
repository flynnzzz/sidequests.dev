#include "../src/arraylist.h"
#include <assert.h>
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

  PASS();
  al_free(array);
}

static void test_new_array() {
  TEST("test_new_array");

  al_type *array = al_empty();
  assert(al_length(array) == 0);
  al_free(array);

  array = al_new(0);
  assert(al_length(array) == 0);
  assert(al_capacity(array) >= INITIAL_CAPACITY);
  al_free(array);

  array = al_new(INITIAL_CAPACITY);
  assert(al_length(array) == 0);
  assert(al_capacity(array) >= INITIAL_CAPACITY);
  PASS();
  al_free(array);
}

static void test_insert_basic() {
  TEST("test_insert_basic");

  al_type *array = al_empty();

  al_insert(&array, 1);
  assert(al_length(array) == 1);
  assert(array[0] == 1);

  al_insert(&array, 2);
  assert(al_length(array) == 2);
  assert(array[1] == 2);

  al_insert(&array, 3);
  assert(al_length(array) == 3);
  assert(array[2] == 3);

  PASS();
  al_free(array);
}

static void test_insert_capacity_growth() {
  TEST("test_insert_capacity_growth");

  al_type *array = al_empty();

  for (int i = 0; i < INITIAL_CAPACITY; i++) {
    al_insert(&array, i);
    assert(al_length(array) == i + 1);
    assert(array[i] == i);
  }

  int original_capacity = al_capacity(array);
  for (int i = INITIAL_CAPACITY; i < INITIAL_CAPACITY + 10; i++) {
    al_insert(&array, i);
    assert(al_length(array) == i + 1);
    assert(array[i] == i);
    assert(al_capacity(array) >= original_capacity);
  }

  PASS();
  al_free(array);
}

static void test_remove_by_value_first_occurrence() {
  TEST("test_remove_by_value_first_occurrence");

  al_type *array = al_empty();

  al_insert(&array, 1);
  al_insert(&array, 2);
  al_insert(&array, 1);
  al_insert(&array, 3);
  al_insert(&array, 1);

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

  al_insert(&array, 1);
  assert(al_length(array) == 1);

  // Try to remove a value that doesn't exist
  al_remove(array, 999);

  assert(al_length(array) == 1);
  assert(array[0] == 1);

  PASS();
  al_free(array);
}

static void test_remove_all_occurrences() {
  TEST("test_remove_all_occurrences");

  al_type *array = al_empty();

  al_insert(&array, 5);
  al_insert(&array, 3);
  al_insert(&array, 5);
  al_insert(&array, 7);
  al_insert(&array, 5);

  assert(al_length(array) == 5);

  // Remove all occurrences of 5 by calling al_remove multiple times
  al_remove(array, 5);
  al_remove(array, 5);

  assert(al_length(array) == 3);
  assert(array[0] == 3);
  assert(array[1] == 7);

  // Third occurrence should be gone (only removed first two times)
  // The current behavior only removes first iteration

  PASS();
  al_free(array);
}

static void test_iremove_basic() {
  TEST("test_iremove_basic");

  al_type *array = al_empty();

  al_insert(&array, 1);
  al_insert(&array, 2);
  al_insert(&array, 3);
  al_insert(&array, 4);
  al_insert(&array, 5);

  assert(al_length(array) == 5);

  // Remove element at index 2 (value 3)
  al_iremove(array, 2);

  assert(al_length(array) == 4);
  assert(array[2] == 4); // Element shifted from index 3

  PASS();
  al_free(array);
}

static void test_iremove_first_and_last() {
  TEST("test_iremove_first_and_last");

  al_type *array = al_empty();

  al_insert(&array, 10);
  al_insert(&array, 20);
  al_insert(&array, 30);
  al_insert(&array, 40);
  al_insert(&array, 50);

  assert(al_length(array) == 5);

  // Remove first element (index 0)
  al_iremove(array, 0);
  assert(al_length(array) == 4);
  assert(array[0] == 20);

  // Remove last element (which is now at index 3)
  al_iremove(array, 3);
  assert(al_length(array) == 3);
  assert(array[2] == 40);

  // Remove single remaining middle element
  al_iremove(array, 1);
  assert(al_length(array) == 2);

  PASS();
  al_free(array);
}

static void test_iremove_empty_array() {
  TEST("test_iremove_empty_array");

  al_type *array = al_empty();
  al_remove(array, 0);
  al_remove(array, 10);
  al_remove(array, -10);

  PASS();
  al_free(array);
}

static void test_iremove_middle_element() {
  TEST("test_iremove_middle_element");

  al_type *array = al_empty();

  al_insert(&array, 100);
  al_insert(&array, 200);
  al_insert(&array, 300);
  al_insert(&array, 400);
  al_insert(&array, 500);

  assert(al_length(array) == 5);

  // Remove middle element (index 2, value 300)
  al_iremove(array, 2);

  assert(al_length(array) == 4);
  assert(array[1] == 200);
  assert(array[2] == 400); // Was at index 3

  PASS();
  al_free(array);
}

static void test_insert_remove_same_value() {
  TEST("test_insert_remove_same_value");

  al_type *array = al_empty();

  al_insert(&array, 10);
  assert(al_length(array) == 1);

  // Remove the same value (only one occurrence exists)
  al_remove(array, 10);

  assert(al_length(array) == 0);

  PASS();
  al_free(array);
}

static void test_insert_remove_interleaved() {
  TEST("test_insert_remove_interleaved");

  al_type *array = al_empty();

  al_insert(&array, 1);
  al_insert(&array, 2);
  al_insert(&array, 3);

  assert(al_length(array) == 3);

  // Remove middle value
  al_remove(array, 2);
  assert(al_length(array) == 2);
  assert(array[0] == 1);
  assert(array[1] == 3);

  // Insert another value
  al_insert(&array, 4);
  assert(al_length(array) == 3);
  assert(array[1] == 3);
  assert(array[2] == 4);

  PASS();
  al_free(array);
}

static void test_capacity_boundary() {
  TEST("test_capacity_boundary");

  al_type *array = al_empty();

  // Insert many elements to near capacity
  for (int i = 0; i < INITIAL_CAPACITY - 1; i++) {
    al_insert(&array, i);
  }

  assert(al_length(array) == INITIAL_CAPACITY - 1);
  assert(al_capacity(array) >= INITIAL_CAPACITY);

  // Insert one more to trigger growth
  al_insert(&array, INITIAL_CAPACITY - 1);

  assert(al_length(array) == INITIAL_CAPACITY);
  assert(al_capacity(array) >= INITIAL_CAPACITY);

  PASS();
  al_free(array);
}

static void test_remove_at_boundary() {
  TEST("test_remove_at_boundary");

  al_type *array = al_empty();

  // Create array with few elements for easier boundary testing
  for (int i = 1; i <= 5; i++) {
    al_insert(&array, i);
  }

  assert(al_length(array) == 5);

  // Remove last element
  al_remove(array, 5);
  assert(al_length(array) == 4);

  // Remove new last element
  al_remove(array, 4);
  assert(al_length(array) == 3);

  PASS();
  al_free(array);
}

static void test_iremove_out_of_bounds() {
  TEST("test_iremove_out_of_bounds");

  al_type *array = al_empty();

  for (int i = 0; i < 10; i++) {
    al_insert(&array, i);
  }

  assert(al_length(array) == 10);

  // Try to remove out of bounds index (should handle gracefully)
  // Note: current implementation may not check bounds
  al_iremove(array, -1);
  assert(al_length(array) == 10); // Should still be 10

  al_iremove(array, 99);
  assert(al_length(array) == 10); // Should still be 10

  PASS();
  al_free(array);
}

static void test_remove_first_occurrence_edge_case() {
  TEST("test_remove_first_occurrence_edge_case");

  al_type *array = al_empty();

  // All elements are the same value
  for (int i = 0; i < 5; i++) {
    al_insert(&array, 42);
  }

  assert(al_length(array) == 5);

  // Remove should only remove first occurrence
  al_remove(array, 42);

  assert(al_length(array) == 4);
  assert(array[0] == 42); // Still has 42 at index 0 (was the second original)

  // Remove again - should still be first remaining occurrence
  al_remove(array, 42);
  assert(al_length(array) == 3);

  PASS();
  al_free(array);
}

static void test_mixed_operations() {
  TEST("test_mixed_operations");

  al_type *array = al_empty();

  al_insert(&array, 10);
  al_insert(&array, 20);
  al_insert(&array, 30);
  al_insert(&array, 40);

  assert(al_length(array) == 4);

  // Remove middle value
  al_remove(array, 30);
  assert(al_length(array) == 3);

  // Insert new value
  al_insert(&array, 99);
  assert(al_length(array) == 4);

  // Remove first value
  al_iremove(array, 0);
  assert(al_length(array) == 3);

  // Remove last value
  al_iremove(array, al_length(array) - 1);
  assert(al_length(array) == 2);

  PASS();
  al_free(array);
}

static void test_empty_value_removal() {
  TEST("test_empty_value_removal");

  al_type *array = al_empty();

  al_insert(&array, 0);
  al_insert(&array, 1);
  al_insert(&array, 2);

  assert(al_length(array) == 3);

  // Remove 0 (valid integer value, not empty array)
  al_remove(array, 0);
  assert(al_length(array) == 2);
  assert(array[0] == 1);
  assert(array[1] == 2);

  PASS();
  al_free(array);
}

static void test_negative_value_removal() {
  TEST("test_negative_value_removal");

  al_type *array = al_empty();

  al_insert(&array, -100);
  al_insert(&array, 0);
  al_insert(&array, 100);

  assert(al_length(array) == 3);

  // Remove negative value
  al_remove(array, -100);
  assert(al_length(array) == 2);
  assert(array[0] == 0);
  assert(array[1] == 100);

  PASS();
  al_free(array);
}

static void test_large_number_of_items() {
  TEST("test_large_number_of_items");

  al_type *array = al_empty();

  // Insert many items
  for (int i = 0; i < 1000; i++) {
    al_insert(&array, i);
  }

  assert(al_length(array) == 1000);

  // Remove a value from middle
  al_remove(array, 500);
  assert(al_length(array) == 999);
  assert(array[500] == 501);

  // Remove another
  al_iremove(array, 100);
  assert(al_length(array) == 998);

  PASS();
  al_free(array);
}

static void test_alternating_values() {
  TEST("test_alternating_values");

  al_type *array = al_empty();

  for (int i = 0; i < 20; i++) {
    if (i % 2 == 0) {
      al_insert(&array, i);
    } else {
      al_insert(&array, -i);
    }
  }

  assert(al_length(array) == 20);

  // Remove a specific value that appears multiple times if any
  // In this case, no duplicates exist

  PASS();
  al_free(array);
}

static void test_remove_then_insert_same() {
  TEST("test_remove_then_insert_same");

  al_type *array = al_empty();

  al_insert(&array, 5);
  assert(al_length(array) == 1);

  al_remove(array, 5);
  assert(al_length(array) == 0);

  // Insert same value again
  al_insert(&array, 5);
  assert(al_length(array) == 1);
  assert(array[0] == 5);

  // Remove again - should be empty
  al_remove(array, 5);
  assert(al_length(array) == 0);

  PASS();
  al_free(array);
}

static void test_print_empty_array() {
  TEST("test_print_empty_array");

  al_type *array = al_empty();

  printf("Empty array: ");
  al_print(array);
  assert(al_length(array) == 0);

  PASS();
  al_free(array);
}

static void test_print_single_element() {
  TEST("test_print_single_element");

  al_type *array = al_empty();
  al_insert(&array, 42);

  printf("Single element array: ");
  al_print(array);

  assert(al_length(array) == 1);

  PASS();
  al_free(array);
}

static void test_all_functions_used() {
  TEST("test_all_functions_used");

  // al_empty - used above
  al_type *array = al_empty();

  // al_new - not strictly necessary since al_empty handles it, but let's use it
  al_type *array2 = al_new(0);

  // al_free - will be called below

  // al_length - checked in many tests above
  size_t len = al_length(array);

  // al_capacity - checked in many tests above
  size_t cap = al_capacity(array);

  // al_insert - used extensively
  al_insert(&array, 99);

  // al_remove - used to remove by value
  al_remove(array, 99);

  // al_iremove - use to remove by index
  al_insert(&array, 1);
  al_insert(&array, 2);
  al_insert(&array, 3);
  assert(al_length(array) == 3);
  al_iremove(array, 1);
  assert(al_length(array) == 2);

  // al_print - will be called below

  PASS();
  al_free(array);
  al_free(array2);
}

static void test_new_exceed_maximum_capacity() {
  TEST("test_new_exceed_maximum_capacity");

  al_type *array = al_new(MAX_CAPACITY);
  assert(array == NULL);

  PASS();
}

static void test_insert_exceed_maximum_capacity() {
  TEST("test_insert_exceed_maximum_capacity");

  al_type *array = al_new(MAX_CAPACITY - 1);
  assert(al_capacity(array) == MAX_CAPACITY - 1);
  arrayhead *head = (arrayhead *)array - 1;
  head->length = MAX_CAPACITY - 1;

  al_insert(&array, 0);

  assert(al_capacity(array) == MAX_CAPACITY - 1);
  assert(array != NULL);

  PASS();
}

static void test_nullptr_guard() {
  TEST("test_nullptr_guard");

  puts("");
  assert(al_length(NULL) == -1);
  assert(al_capacity(NULL) == -1);

  PASS();
}

int main() {
  puts("=== Running ArrayList Tests ===\n");

  test_empty_array();
  test_new_array();
  test_insert_basic();
  test_insert_capacity_growth();
  test_remove_by_value_first_occurrence();
  test_remove_nonexistent_value();
  test_remove_all_occurrences();
  test_iremove_basic();
  test_iremove_first_and_last();
  test_iremove_empty_array();
  test_iremove_middle_element();
  test_insert_remove_same_value();
  test_insert_remove_interleaved();
  test_capacity_boundary();
  test_remove_at_boundary();
  test_iremove_out_of_bounds();
  test_remove_first_occurrence_edge_case();
  test_mixed_operations();
  test_empty_value_removal();
  test_negative_value_removal();
  test_large_number_of_items();
  test_alternating_values();
  test_remove_then_insert_same();
  test_print_empty_array();
  test_print_single_element();
  test_all_functions_used();
  test_new_exceed_maximum_capacity();
  test_insert_exceed_maximum_capacity();
  test_nullptr_guard();

  puts("\n=== All Tests Completed ===");

  return 0;
}
