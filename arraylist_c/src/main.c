/*
 * main.c
 *
 * API showcase.
 */

#include <stdio.h>

#include "arraylist.h"

int main() {

  al_type *array = al_empty();

  puts("al_push():");
  puts(" al_push(&array, 100);\n"
       " al_push(&array, 101);\n"
       " al_push(&array, 102);");

  al_push(&array, 100);
  al_push(&array, 101);
  al_push(&array, 102);

  puts("\nal_print():");
  al_print(array);

  puts("\naccessing items:");
  printf(" array[0]: %d\n", array[0]);
  printf(" array[1]: %d\n", array[1]);
  printf(" array[2]: %d\n", array[2]);

  puts("\nmodifying items:");
  array[0] = 103;
  array[1] = 104;
  array[2] = 105;
  printf("array[0] = %d, array[1] = %d, array[2] = %d\n", 103, 104, 105);
  printf(" array[0]: %d\n", array[0]);
  printf(" array[1]: %d\n", array[1]);
  printf(" array[2]: %d\n", array[2]);
  puts("al_print():");
  al_print(array);

  al_free(array);
  return 0;
}
