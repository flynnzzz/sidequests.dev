#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <unistd.h>

#define INITIAL_CAPACITY 32
#define GROWTH_FACTOR 2

typedef struct arrayhead_t {
  size_t lenght, capacity;
} arrayhead;

typedef int al_type;

al_type *al_empty();

al_type *al_new(size_t lenght);

size_t al_lenght(const al_type array[]);

size_t al_capacity(const al_type array[]);

void al_insert(al_type array[], const al_type item);

void al_remove(al_type array[], const al_type item);

void al_iremove(al_type array[], int index);

void al_print(const al_type array[]);

#endif // !ARRAYLIST_H
