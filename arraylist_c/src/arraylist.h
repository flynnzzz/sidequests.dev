/*
 * arraylist.h
 *
 * Dynamic array interface.
 *
 * This implementation makes use of the `hidden header` method by
 * storing metadata right before the start of the first item.
 * NOTE: Not yet implemented:
 * The API Achieves generics like behaviour through the use of macros,
 * exchanging code length for simplicity and compile time type checking.
 */

#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stddef.h>

#define INITIAL_CAPACITY 32
#define MAX_SIZE ((~(size_t)0) >> 1)
#define MAX_CAPACITY ((MAX_SIZE - sizeof(arrayhead)) / sizeof(al_type))
#define GROWTH_FACTOR 2

typedef int al_type;

typedef struct arrayhead_t {
  size_t length, capacity;
} arrayhead;

al_type *al_empty();

al_type *al_create(size_t length);

int al_free(al_type array[]);

size_t al_length(const al_type array[]);

size_t al_capacity(const al_type array[]);

int al_push(al_type *arrayptr[], const al_type item);

al_type al_pop(al_type array[]);

int al_insert(al_type array[], size_t index, al_type item);

int al_contains(al_type array[], al_type item);

size_t al_find(al_type array[], al_type item);

int al_remove(al_type array[], const al_type item);

// TODO: change so it returns the removed item

int al_iremove(al_type array[], size_t index);

int al_print(const al_type array[]);

int al_clear(al_type array[]);

int al_fclear(al_type array[]);

al_type *al_copy(al_type array[]);

int al_resize(al_type array[], size_t capacity);

int al_grow(al_type array[]);

int al_swap(al_type array[], size_t index1, size_t index2);

int al_sort(al_type array[], int (*comparator)(al_type, al_type));

int al_reverse(al_type array[]);

#endif // !ARRAYLIST_H
