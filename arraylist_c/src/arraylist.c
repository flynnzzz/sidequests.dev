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

#define alH_head(array) ((alH_head *)(array) - 1)
#define alH_skiphead(head) ((T *)((alH_head *)(head) + 1))

static inline int alH_isnullptr(const void *pointer, const char *message) {
  if (pointer == NULL) {
    fprintf(stderr, "%s\n", message);
    return 1;
  }
  return 0;
}

static void alH_lshift(T array[], size_t start, size_t end) {
  if (alH_isnullptr(array, "al_shift: NULL pointer"))
    return;

  for (size_t i = start; i < end && i < al_length(array) - 1; i++) {
    array[i] = array[i + 1];
  }
  alH_head(array)->length--;
}

static void alH_rshift(T array[], size_t start, size_t end) {
  if (alH_isnullptr(array, "al_shift: NULL pointer"))
    return;

  for (size_t i = start; i < end && i < al_length(array) - 1; i++) {
    array[i + 1] = array[i];
  }
  alH_head(array)->length--;
}

inline T *al_empty() { return al_create(0); }

T *al_create(size_t capacity) {
  if (capacity >= MAX_CAPACITY) {
    fputs("al_new: `len` exceeds maximum capacity\n", stderr);
    return NULL;
  }

  capacity = capacity == 0 ? INITIAL_CAPACITY : capacity;
  alH_head *head =
      (alH_head *)malloc(sizeof(alH_head) + (capacity * sizeof(T)));
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

inline int al_free(T *array) {
  if (alH_isnullptr(array, "al_free: NULL pointer"))
    return AL_FAIL;
  free(alH_head(array));
  return AL_SUCCESS;
}

inline size_t al_length(const T array[]) {
  if (alH_isnullptr(array, "al_length: NULL pointer"))
    return AL_ERRSIZE;
  return alH_head(array)->length;
}

inline size_t al_capacity(const T array[]) {
  if (alH_isnullptr(array, "al_capacity: NULL pointer"))
    return AL_ERRSIZE;
  return alH_head(array)->capacity;
}

int al_push(T *arrayptr[], const T item) {
  if (alH_isnullptr(arrayptr, "al_insert: NULL pointer") ||
      alH_isnullptr(*arrayptr, "al_insert: NULL pointer"))
    return AL_FAIL;

  alH_head *head = alH_head(*arrayptr);

  if (head->length + 1 >= MAX_CAPACITY) {
    fputs("al_insert: array reached maximum capacity\n", stderr);
    return AL_FAIL;
  }

  if (head->length >= head->capacity) {
    size_t new_capacity = head->capacity * GROWTH_FACTOR;

    new_capacity =
        (new_capacity >= MAX_CAPACITY) ? head->capacity : new_capacity;

    alH_head *new_head =
        realloc(head, sizeof(alH_head) + sizeof(T) * new_capacity);
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

inline T al_pop(T array[]) { return al_rmfirst(array, al_length(array) - 1); }

int al_insert(T *arrayptr[], size_t index, T item) {
  // TODO: test
  T *array = *arrayptr;
  int err = al_push(arrayptr, item);
  if (err == AL_FAIL) {
    return err;
  }

  alH_rshift(array, index, al_length(array) - 2);
  array[index] = array[al_length(array) - 1];

  return err;
}

T al_rmfirst(T array[], const T item) {
  if (!alH_isnullptr(array, "al_remove: NULL pointer")) {
    for (size_t i = 0; i < al_length(array); i++) {
      if (array[i] == item) {
        alH_lshift(array, i, al_length(array) - 1);
        return AL_SUCCESS;
      }
    }
  }
  return AL_ERRTYPE;
}

int al_rmall(T array[], const T item) {
  // TODO: test
  size_t found = 0;
  if (!alH_isnullptr(array, "al_remove: NULL pointer")) {
    for (size_t i = 0; i < al_length(array); i++) {
      if (array[i] == item) {
        alH_lshift(array, i, al_length(array) - 1);
        found++;
      }
    }
  }
  return (found > 0) ? AL_SUCCESS : AL_FAIL;
}

inline T al_rmat(T array[], size_t index) {
  if (alH_isnullptr(array, "al_iremove: NULL pointer") ||
      index >= al_length(array))
    return AL_ERRTYPE;

  alH_lshift(array, index, al_length(array) - 1);
  return AL_SUCCESS;
}

int al_print(const T array[]) {
  if (alH_isnullptr(array, "al_print: NULL pointer"))
    return AL_FAIL;

  alH_head *head = alH_head(array);
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
