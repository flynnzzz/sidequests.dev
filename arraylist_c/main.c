#include "arraylist.h"
#include <assert.h>
#include <stdio.h>

int main() {
  int *array0 = al_empty();
  assert(al_lenght(array0) == 0);

  al_insert(array0, 8);
  al_insert(array0, 8);
  al_insert(array0, 8);

  assert(al_lenght(array0) == 3);

  al_print(array0);

  int *array1 = al_new(1);
  assert(al_lenght(array1) == 0);

  al_insert(array1, 9);
  assert(al_lenght(array1) == 1);

  al_print(array1);

  return 0;
}
