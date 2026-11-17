/*
 * arraylist.c
 *
 * NOTE: Temporary: implementation for arraylist.h
 */

#define AL_ENABLE_TESTS
#include "arraylist.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define alH_head(array) ((alH_head *)(array) - 1)
#define alH_skiphead(head) ((T *)((alH_head *)(head) + 1))
#define AL_MAXBUF 256

static inline void alH_log(const char *message) { fputs(message, stderr); }

static inline int alH_isnullptr(const void *pointer, const char *message) {
  if (pointer == NULL) {
    fputs(message, stderr);
    return 1;
  }
  return 0;
}

/*
 * Simply returns error `code` if `AL_ENABLE_TESTS` is defined, else aborts the
 * program.
 */
static inline int alH_fail(int code) {
#ifndef AL_ENABLE_TESTS
  exit(EXIT_FAILURE);
#endif
  return code;
}

/*
 * Sets items from `start` to `end` to `c`.
 */
static inline void alH_set(T array[], T c, size_t start, size_t end) {
  memset(array + start, c, (end - start) * sizeof(T));
}

/*
 * [start, end[
 */
static int alH_lshift(T array[], size_t start, size_t end, int step) {
  if (alH_isnullptr(array, "alH_lshift: NULL pointer\n"))
    return alH_fail(AL_FAIL);
  if (end < start) {
    fputs("alH_lshift: `end` < `start`\n", stderr);
    return alH_fail(AL_FAIL);
  }

  for (size_t i = start; i < al_length(array) - step && i < end - step; i++) {
    array[i] = array[i + step];
  }

  return AL_SUCCESS;
}

/*
 * [start, end[
 */
static int alH_rshift(T array[], size_t start, size_t end, int step) {
  if (alH_isnullptr(array, "alH_rshift: NULL pointer\n"))
    return alH_fail(AL_FAIL);
  if (end < start) {
    alH_log("alH_rshift: `end` < `start`\n");
    return alH_fail(AL_FAIL);
  }

  for (size_t i = end - step; i > start; i--) {
    array[i] = array[i - step];
  }

  return AL_SUCCESS;
}

inline T *al_init() { return al_create(0); }

T *al_create(size_t capacity) {
  if (capacity >= AL_MAXCAP) {
    alH_log("al_create: `capacity` exceeds maximum capacity\n");
    return NULL;
  }

  capacity = capacity == 0 ? AL_INIT_CAPACITY : capacity;
  alH_head *head =
      (alH_head *)malloc(sizeof(alH_head) + (capacity * sizeof(T)));
  if (!head) {
    perror("al_new");
    return NULL;
  }
  head->capacity = capacity;
  head->length = 0;

  return alH_skiphead(head);
}

inline int al_free(T *array) {
  if (alH_isnullptr(array, "al_free: NULL pointer\n"))
    return alH_fail(AL_FAIL);

  free(alH_head(array));
  return AL_SUCCESS;
}

inline size_t al_length(const T array[]) {
  return (alH_isnullptr(array, "al_length: NULL pointer\n"))
             ? AL_ERRSIZE
             : alH_head(array)->length;
}

inline size_t al_capacity(const T array[]) {
  return (alH_isnullptr(array, "al_capacity: NULL pointer\n"))
             ? AL_ERRSIZE
             : alH_head(array)->capacity;
}

inline int al_grow(T *arrptr[]) {
  if (alH_isnullptr(arrptr, "al_grow: NULL pointer\n") ||
      alH_isnullptr(*arrptr, "al_grow: pointer to array points to NULL\n"))
    return alH_fail(AL_FAIL);

  return al_resize(arrptr, alH_head(*arrptr)->capacity * AL_GROWTH_FACTOR);
}

int al_resize(T *arrptr[], size_t capacity) {
  if (alH_isnullptr(arrptr, "al_resize: NULL pointer\n") ||
      alH_isnullptr(*arrptr, "al_resize: pointer to array points to NULL\n"))
    return alH_fail(AL_FAIL);
  if (capacity >= AL_MAXCAP) {
    alH_log("al_resize: `capacity` exceeds maximum capacity\n");
    return alH_fail(AL_FAIL);
  }
  alH_head *head = alH_head(*arrptr), *new_head;
  size_t oldlen = head->length;

  capacity = (capacity == 0) ? AL_INIT_CAPACITY : capacity;
  new_head = realloc(head, sizeof(alH_head) + sizeof(T) * capacity);
  if (!new_head) {
    perror("al_resize");
    free(head);
    alH_fail(AL_FAIL);
  }

  new_head->capacity = capacity;
  new_head->length = (capacity < oldlen) ? capacity : oldlen;

  *arrptr = alH_skiphead(new_head);
  alH_set(*arrptr, 0, al_length(*arrptr), al_capacity(*arrptr));
  return AL_SUCCESS;
}

int al_push(T *arrptr[], const T item) {
  if (alH_isnullptr(arrptr, "al_push: NULL pointer\n") ||
      alH_isnullptr(*arrptr, "al_push: pointer to array points to NULL\n"))
    return alH_fail(AL_FAIL);

  T *array = *arrptr;

  alH_head *head = alH_head(array);
  if (head->length + 1 >= AL_MAXCAP) {
    alH_log("al_push: array reached maximum capacity\n");
    return alH_fail(AL_FAIL);
  }

  if (head->length >= head->capacity) {
    al_grow(arrptr);
    array = *arrptr;
  }

  array[alH_head(array)->length++] = item;

  return AL_SUCCESS;
}

inline T al_pop(T array[]) { return al_rm(array, al_length(array) - 1); }

int al_insert(T *arrptr[], size_t index, const T item) {
  if (alH_isnullptr(arrptr, "al_insert: NULL pointer\n") ||
      alH_isnullptr(*arrptr, "al_insert: pointer to array points to NULL\n"))
    return alH_fail(AL_FAIL);

  if (index > al_length(*arrptr)) {
    alH_log("al_insert: `index` exceeds arraylist length\n");
    return alH_fail(AL_FAIL);
  }
  if (!al_push(arrptr, item) ||
      !alH_rshift(*arrptr, index, al_length(*arrptr), 1))
    return alH_fail(AL_FAIL);

  (*arrptr)[index] = item;
  return AL_SUCCESS;
}

T al_rm(T array[], const T item) {
  if (alH_isnullptr(array, "al_rm: NULL pointer\n")) {
#ifndef AL_ENABLE_TESTS
    exit(EXIT_FAILURE);
#endif
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
  if (alH_isnullptr(array, "al_rmall: NULL pointer\n")) {
    return alH_fail(AL_FAIL);
  }
  size_t i = 0;
  while (i < al_length(array)) {
    if (array[i] == item) {
      size_t found = 1;

      for (size_t j = i; j < al_length(array) - 1 && array[j + 1] == item; j++)
        found++;

      alH_lshift(array, i, al_length(array), found);

      alH_head(array)->length -= found;
      removed += found;
    } else
      i++;
  }
  alH_set(array, 0, al_length(array), al_capacity(array));
  return removed;
}

inline T al_rmat(T array[], size_t index) {
  if (alH_isnullptr(array, "al_rmat: NULL pointer\n")) {
#ifndef AL_ENABLE_TESTS
    exit(EXIT_FAILURE);
#endif
    return AL_ERRTYPE;
  }
  if (index >= al_length(array)) {
    alH_log("al_rmat: `index` exceeds array length\n");
#ifndef AL_ENABLE_TESTS
    exit(EXIT_FAILURE);
#endif
    return AL_ERRTYPE;
  }

  alH_lshift(array, index, al_length(array), 1);
  array[al_length(array) - 1] = 0;
  alH_head(array)->length--;
  return AL_SUCCESS;
}

int al_print(const T array[], int (*to_string)(T, char *)) {
  if (alH_isnullptr(array, "al_print: NULL pointer\n"))
    return alH_fail(AL_FAIL);

  alH_head *head = alH_head(array);
  char buffer[AL_MAXBUF] = "\0";
  printf("{ ");
  for (size_t i = 0; i < head->length; i++) {
    if (to_string(array[i], buffer))
      printf("%s ", buffer);
    else {
      alH_log("al_print: string conversion failed");
      return AL_FAIL;
    }
  }
  puts("}");

  return AL_SUCCESS;
}

inline int al_clear(T array[]) {
  // TODO: test
  if (alH_isnullptr(array, "al_clear: NULL pointer\n"))
    return alH_fail(AL_FAIL);

  alH_set(array, 0, 0, al_capacity(array));
  alH_head(array)->length = 0;
  return AL_SUCCESS;
}
