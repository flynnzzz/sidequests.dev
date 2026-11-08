#include "arraylist.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define al_head(array) ((arrayhead *)(array - (char)sizeof(arrayhead)))

al_type *al_empty() { return al_new(0); }

al_type *al_new(size_t len) {
  al_type *array =
      (al_type *)malloc(sizeof(arrayhead) + (len * sizeof(al_type)));
  if (!array) {
    perror("al_new");
    exit(EXIT_FAILURE);
  }
  arrayhead *head = (arrayhead *)array;
  head->capacity = len == 0 ? INITIAL_CAPACITY : len;
  head->lenght = 0;

  array = array + (char)sizeof(arrayhead);
  return array;
}

size_t al_lenght(const al_type array[]) {
  arrayhead *head = al_head(array);
  return head->lenght;
}

size_t al_capacity(const al_type array[]) {
  arrayhead *head = al_head(array);
  return head->capacity;
}

void al_insert(al_type array[], const al_type item) {
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

static void lshift(al_type array[], int start) {
  for (int i = start; i < al_lenght(array) - 1; i++) {
    array[i] = array[i + 1];
  }
  al_head(array)->lenght--;
}

void al_remove(al_type array[], const al_type item) {
  for (int i = 0; i < al_lenght(array); i++) {
    if (array[i] == item) {
      lshift(array, i);
      return;
    }
  }
}

inline void al_iremove(al_type array[], int index) { lshift(array, index); }

void al_print(const al_type array[]) {
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
