/*
 * arraylist.c
 *
 * NOTE: Temporary: implementation for arraylist.h
 */

#include <assert.h>
#include <string.h>
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

/*
 * [start, end[
 */
static int alH_lshift(T array[], size_t start, size_t end, int step) {
  if (alH_isnullptr(array, "alH_lshift: NULL pointer"))
    return AL_FAIL;
  if (end < start) {
    fputs("alH_lshift: `end` < `start`\n", stderr);
    return AL_FAIL;
  }

  for (size_t i = start; i < end - step && i < al_length(array) - step; i++) {
    array[i] = array[i + step];
  }

  return AL_SUCCESS;
}

/*
 * [start, end[
 */
static int alH_rshift(T array[], size_t start, size_t end, int step) {
  if (alH_isnullptr(array, "alH_rshift: NULL pointer"))
    return AL_FAIL;
  if (end < start) {
    fputs("alH_rshift: `end` < `start`\n", stderr);
    return AL_FAIL;
  }

  for (size_t i = end - step; i > start; i--) {
    array[i] = array[i - step];
  }

  return AL_SUCCESS;
}

inline T *al_init() { return al_create(0); }

T *al_create(size_t capacity) {
  if (capacity >= MAX_CAPACITY) {
    fputs("al_create: `capacity` exceeds maximum capacity\n", stderr);
    return NULL;
  }

  capacity = capacity == 0 ? INITIAL_CAPACITY : capacity;
  alH_head *head =
      (alH_head *)malloc(sizeof(alH_head) + (capacity * sizeof(T)));
  if (!head) {
    perror("al_new");

#ifndef AL_DEBUG
    exit(EXIT_FAILURE);
#else
    return NULL;
#endif
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
  if (alH_isnullptr(arrayptr, "al_push: NULL pointer") ||
      alH_isnullptr(*arrayptr, "al_push: pointer to array points to NULL"))
    return AL_FAIL;

  alH_head *head = alH_head(*arrayptr);

  if (head->length + 1 >= MAX_CAPACITY) {
    fputs("al_push: array reached maximum capacity\n", stderr);
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
#else
      return AL_FAIL;
#endif
    }
    new_head->capacity = new_capacity;
    head = new_head;
  }

  *arrayptr = alH_skiphead(head);
  (*arrayptr)[head->length++] = item;

  return AL_SUCCESS;
}

inline T al_pop(T array[]) { return al_rm(array, al_length(array) - 1); }

int al_insert(T *arrayptr[], size_t index, const T item) {
  if (alH_isnullptr(arrayptr, "al_insert: NULL pointer") ||
      alH_isnullptr(*arrayptr, "al_insert: pointer to array points to NULL"))
    return AL_FAIL;
  T *array = *arrayptr;
  if (index > al_length(array)) {
    fputs("al_insert: `index` exceeds arraylist length\n", stderr);
    return AL_FAIL;
  }
  if (!al_push(arrayptr, item)) {
    return AL_FAIL;
  }
  if (!alH_rshift(array, index, al_length(array), 1)) {
    return AL_FAIL;
  }

  array[index] = item;
  return AL_SUCCESS;
}

T al_rm(T array[], const T item) {
  if (alH_isnullptr(array, "al_rm: NULL pointer")) {
    return AL_ERRTYPE;
  }
  for (size_t i = 0; i < al_length(array); i++) {
    if (array[i] == item) {
      alH_lshift(array, i, al_length(array), 1);
      array[al_length(array) - 1] = 0;
      alH_head(array)->length--;
      return AL_SUCCESS;
    }
  }
  return AL_ERRTYPE;
}

int al_rmall(T array[], const T item) {
  int removed = 0;
  if (alH_isnullptr(array, "al_rmall: NULL pointer")) {
    return AL_FAIL;
  }

  for (size_t i = 0; i < al_length(array); i++) {
    if (array[i] == item) {
      size_t found = 1;

      for (size_t j = i; array[j + 1] == item && j < al_length(array) - 1; j++)
        found++;

      alH_lshift(array, i, al_length(array), found);

      alH_head(array)->length -= found;
      removed += found;
    }
  }
  memset(&(array[al_length(array)]), 0, removed * sizeof(T));
  return removed;
}

inline T al_rmat(T array[], size_t index) {
  if (alH_isnullptr(array, "al_rmat: NULL pointer") ||
      index >= al_length(array))
    return AL_ERRTYPE;

  alH_lshift(array, index, al_length(array), 1);
  array[al_length(array) - 1] = 0;
  alH_head(array)->length--;
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
