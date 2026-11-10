#include "arraylist.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define al_head(array) ((arrayhead *)(array) - 1)
#define skip_head(head) ((al_type *)((arrayhead *)(head) + 1))

#define nullptr_guard(pointer, message)                                        \
  do {                                                                         \
    if (pointer == NULL) {                                                     \
      fputs(message, stderr);                                                  \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

al_type *al_empty() { return al_new(0); }

al_type *al_new(size_t len) {
  if (len >= MAX_CAPACITY) {
    fputs("al_new: `len` exceeds maximum capacity\n", stderr);
    return NULL;
  }

  len = len == 0 ? INITIAL_CAPACITY : len;
  al_type *array =
      (al_type *)malloc(sizeof(arrayhead) + (len * sizeof(al_type)));
  if (!array) {
    perror("al_new");
    return NULL;
  }
  arrayhead *head = (arrayhead *)array;
  head->capacity = len;
  head->length = 0;

  return skip_head(head);
}

void al_free(al_type *array) {
  nullptr_guard(array, "al_free: NULL pointer");
  free(al_head(array));
}

size_t al_length(const al_type array[]) {
  nullptr_guard(array, "al_length: NULL pointer");

  arrayhead *head = al_head(array);
  return head->length;
}

size_t al_capacity(const al_type array[]) {
  nullptr_guard(array, "al_capacity: NULL pointer");
  arrayhead *head = al_head(array);
  return head->capacity;
}

void al_insert(al_type *arrayptr[], const al_type item) {
  nullptr_guard(arrayptr, "al_insert: NULL pointer");

  arrayhead *head = al_head(*arrayptr);

  if (head->length + 1 >= MAX_CAPACITY) {
    fputs("al_insert: array reached maximum capacity\n", stderr);
    return;
  }

  if (head->length >= head->capacity) {
    size_t new_capacity = head->capacity * GROWTH_FACTOR;
    arrayhead *new_head =
        realloc(head, sizeof(arrayhead) + sizeof(al_type) * new_capacity);
    if (!new_head) {
      perror("al_insert");
      exit(EXIT_FAILURE);
    }
    new_head->capacity = new_capacity;
    head = new_head;
  }

  *arrayptr = skip_head(head);
  (*arrayptr)[head->length++] = item;
}

static void lshift(al_type array[], int start) {
  nullptr_guard(array, "al_shift: NULL pointer");

  for (int i = start; i < al_length(array) - 1; i++) {
    array[i] = array[i + 1];
  }
  al_head(array)->length--;
}

void al_remove(al_type array[], const al_type item) {
  nullptr_guard(array, "al_remove: NULL pointer");

  for (int i = 0; i < al_length(array); i++) {
    if (array[i] == item) {
      lshift(array, i);
      return;
    }
  }
}

inline void al_iremove(al_type array[], int index) {
  nullptr_guard(array, "al_iremove: NULL pointer");

  if (index >= al_length(array) || index < 0)
    return;

  lshift(array, index);
}

void al_print(const al_type array[]) {
  nullptr_guard(array, "al_print: NULL pointer");

  arrayhead *head = al_head(array);

  printf("{ ");
  for (int i = 0; i < head->length; i++) {
    if (i != head->length - 1)
      printf("%d, ", array[i]);
    else
      printf("%d", array[i]);
  }

  puts(" }");
}
