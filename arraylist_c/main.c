#include "arraylist.h"
#include <assert.h>

static void test() {
  al_type *array0 = al_empty();
  assert(al_length(array0) == 0);
  assert(al_capacity(array0) == INITIAL_CAPACITY);

  al_insert(array0, 8);
  al_insert(array0, 8);
  al_insert(array0, 8);

  assert(al_length(array0) == 3);

  al_print(array0);

  al_type *array1 = al_new(1);
  assert(al_length(array1) == 0);

  al_insert(array1, 9);
  assert(al_length(array1) == 1);

  al_print(array1);
}

static void remove_test() {
  al_type *array = al_empty();
  al_insert(array, 1);
  al_insert(array, 2);
  al_insert(array, 3);
  al_insert(array, 4);
  assert(al_length(array) == 4);

  al_remove(array, 3);
  al_print(array);
  assert(al_length(array) == 3);
  al_iremove(array, 0);
  al_print(array);
  assert(al_length(array) == 2);
  al_remove(array, 4);
  al_print(array);
  assert(al_length(array) == 1);
  al_iremove(array, 0);
  al_print(array);
  assert(al_length(array) == 0);

  al_free(array);
}

int main() {
  test();
  remove_test();
  return 0;
}
