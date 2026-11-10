#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stddef.h>

#define INITIAL_CAPACITY 32
#define GROWTH_FACTOR 2

typedef int al_type;

typedef struct arrayhead_t {
  size_t length, capacity;
  al_type *start;
} arrayhead;

al_type *al_empty();

al_type *al_new(size_t length);

void al_free(al_type array[]);

size_t al_length(const al_type array[]);

size_t al_capacity(const al_type array[]);

void al_insert(al_type *array[], const al_type item);

void al_remove(al_type array[], const al_type item);

void al_iremove(al_type array[], int index);

void al_print(const al_type array[]);

#endif // !ARRAYLIST_H
