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

// TODO: change return types to int to signal error statuses
//
void al_free(al_type array[]);

size_t al_length(const al_type array[]);

size_t al_capacity(const al_type array[]);

void al_push(al_type *arrayptr[], const al_type item);

void al_remove(al_type array[], const al_type item);

void al_iremove(al_type array[], size_t index);

void al_print(const al_type array[]);

#endif // !ARRAYLIST_H
