/*
 * arraylist.c
 *
 * NOTE: Temporary: implementation for arraylist.h
 */

#define AL_DEBUG

#include "arraylist.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define alH_head(array) ((arrayhead *)(array) - 1)
#define alH_skiphead(head) ((al_type *)((arrayhead *)(head) + 1))

enum alH_errstatus { AL_FAIL, AL_SUCCESS };

static inline int alH_isnullptr(const void *pointer, const char *message) {
  if (pointer == NULL) {
    fprintf(stderr, "%s\n", message);
    return 1;
  }
  return 0;
}

static void alH_lshift(al_type array[], int start) {
  if (alH_isnullptr(array, "al_shift: NULL pointer"))
    return;

  for (size_t i = start; i < al_length(array) - 1; i++) {
    array[i] = array[i + 1];
  }
  alH_head(array)->length--;
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

#ifndef AL_DEBUG
    exit(EXIT_FAILURE);
#endif

    return NULL;
  }
  head->capacity = capacity;
  head->length = 0;

  return alH_skiphead(head);
}

inline int al_free(al_type *array) {
  if (alH_isnullptr(array, "al_free: NULL pointer"))
    return AL_FAIL;
  free(alH_head(array));
  return AL_SUCCESS;
}

inline size_t al_length(const al_type array[]) {
  if (alH_isnullptr(array, "al_length: NULL pointer"))
    return AL_FAIL;
  return alH_head(array)->length;
}

inline size_t al_capacity(const al_type array[]) {
  if (alH_isnullptr(array, "al_capacity: NULL pointer"))
    return AL_FAIL;
  return alH_head(array)->capacity;
}

int al_push(al_type *arrayptr[], const al_type item) {
  if (alH_isnullptr(arrayptr, "al_insert: NULL pointer"))
    return AL_FAIL;

  arrayhead *head = alH_head(*arrayptr);

  if (head->length + 1 >= MAX_CAPACITY) {
    fputs("al_insert: array reached maximum capacity\n", stderr);
    return AL_FAIL;
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

#ifndef AL_DEBUG
      exit(EXIT_FAILURE);
#endif
      return AL_FAIL;
    }
    new_head->capacity = new_capacity;
    head = new_head;
  }

  *arrayptr = alH_skiphead(head);
  (*arrayptr)[head->length++] = item;

  return AL_SUCCESS;
}

int al_remove(al_type array[], const al_type item) {
  if (!alH_isnullptr(array, "al_remove: NULL pointer")) {
    for (size_t i = 0; i < al_length(array); i++) {
      if (array[i] == item) {
        alH_lshift(array, i);
        return AL_SUCCESS;
      }
    }
  }

  return AL_FAIL;
}

inline int al_iremove(al_type array[], size_t index) {
  if (alH_isnullptr(array, "al_iremove: NULL pointer"))
    return AL_FAIL;
  if (index >= al_length(array))
    return AL_FAIL;
  alH_lshift(array, index);
  return AL_SUCCESS;
}

int al_print(const al_type array[]) {
  if (alH_isnullptr(array, "al_print: NULL pointer"))
    return AL_FAIL;

  arrayhead *head = alH_head(array);
  printf("{ ");
  size_t last_i = head->length - 1;
  for (size_t i = 0; i < head->length; i++) {
    if (i != last_i)
      printf("%d, ", array[i]);
    else
      printf("%d", array[i]);
  }
  puts(" }");

  return AL_SUCCESS;
}
