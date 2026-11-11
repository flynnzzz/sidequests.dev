/*
 * arraylist.c
 *
 * NOTE: Temporary: implementation for arraylist.h
 */

#include "arraylist.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define al_head(array) ((arrayhead *)(array) - 1)
#define skip_head(head) ((al_type *)((arrayhead *)(head) + 1))

static inline int alH_checknullptr(const void *pointer, const char *message) {
  if (pointer == NULL) {
    fprintf(stderr, "%s\n", message);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static void alH_lshift(al_type array[], int start) {
  if (alH_checknullptr(array, "al_shift: NULL pointer"))
    return;

  for (size_t i = start; i < al_length(array) - 1; i++) {
    array[i] = array[i + 1];
  }
  al_head(array)->length--;
}

inline al_type *al_empty() { return al_create(0); }

al_type *al_create(size_t capacity) {
  if (capacity >= MAX_CAPACITY) {
    fputs("al_new: `len` exceeds maximum capacity\n", stderr);
    return NULL;
  }

  capacity = capacity == 0 ? INITIAL_CAPACITY : capacity;
  arrayhead *head =
      (arrayhead *)malloc(sizeof(arrayhead) + (capacity * sizeof(al_type)));
  if (!head) {
    perror("al_new");
    return NULL;
  }
  head->capacity = capacity;
  head->length = 0;

  return skip_head(head);
}

inline void al_free(al_type *array) {
  if (alH_checknullptr(array, "al_free: NULL pointer"))
    return;
  free(al_head(array));
}

inline size_t al_length(const al_type array[]) {
  if (alH_checknullptr(array, "al_length: NULL pointer"))
    return 0;
  return al_head(array)->length;
}

inline size_t al_capacity(const al_type array[]) {
  if (alH_checknullptr(array, "al_capacity: NULL pointer"))
    return 0;
  return al_head(array)->capacity;
}

void al_push(al_type *arrayptr[], const al_type item) {
  if (alH_checknullptr(arrayptr, "al_insert: NULL pointer"))
    return;

  arrayhead *head = al_head(*arrayptr);

  if (head->length + 1 >= MAX_CAPACITY) {
    fputs("al_insert: array reached maximum capacity\n", stderr);
    return;
  }

  if (head->length >= head->capacity) {
    size_t new_capacity = head->capacity * GROWTH_FACTOR;

    new_capacity =
        (new_capacity >= MAX_CAPACITY) ? head->capacity : new_capacity;

    arrayhead *new_head =
        realloc(head, sizeof(arrayhead) + sizeof(al_type) * new_capacity);
    if (!new_head) {
      perror("al_insert");
      free(head);
      return;
    }
    new_head->capacity = new_capacity;
    head = new_head;
  }

  *arrayptr = skip_head(head);
  (*arrayptr)[head->length++] = item;
}

void al_remove(al_type array[], const al_type item) {
  if (alH_checknullptr(array, "al_remove: NULL pointer"))
    return;

  for (size_t i = 0; i < al_length(array); i++) {
    if (array[i] == item) {
      alH_lshift(array, i);
      return;
    }
  }
}

inline void al_iremove(al_type array[], size_t index) {
  if (alH_checknullptr(array, "al_iremove: NULL pointer"))
    return;
  if (index >= al_length(array))
    return;
  alH_lshift(array, index);
}

void al_print(const al_type array[]) {
  if (alH_checknullptr(array, "al_print: NULL pointer"))
    return;

  arrayhead *head = al_head(array);
  printf("{ ");
  size_t last_i = head->length - 1;
  for (size_t i = 0; i < head->length; i++) {
    if (i != last_i)
      printf("%d, ", array[i]);
    else
      printf("%d", array[i]);
  }
  puts(" }");
}
