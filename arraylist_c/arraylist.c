#include "arraylist.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define al_head(array) ((arrayhead *)(array - (char)sizeof(arrayhead)))

int *al_empty() { return al_new(0); }

int *al_new(size_t len) {
  int *array = (int *)malloc(sizeof(arrayhead) + (len * sizeof(int)));
  if (!array) {
    perror("al_new");
    exit(EXIT_FAILURE);
  }
  arrayhead *head = (arrayhead *)array;
  head->capacity = INITIAL_CAPACITY;
  head->lenght = 0;

  array = array + (char)sizeof(arrayhead);
  return array;
}

size_t al_lenght(const int array[]) {
  arrayhead *head = al_head(array);
  return head->lenght;
}

void al_insert(int array[], const int item) {
  arrayhead *head = al_head(array);

  if (head->lenght == head->capacity) {
    head = realloc(head, sizeof(arrayhead) + (head->capacity * GROWTH_FACTOR));
    if (!head) {
      perror("al_insert");
      exit(EXIT_FAILURE);
    }
  }

  array[head->lenght++] = item;
}

void al_print(const int array[]) {
  arrayhead *head = al_head(array);

  printf("{ ");
  for (int i = 0; i < head->lenght; i++) {
    if (i != head->lenght - 1)
      printf("%d, ", array[i]);
    else
      printf("%d", array[i]);
  }

  puts(" }");
}
