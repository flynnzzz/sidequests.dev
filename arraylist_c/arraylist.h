#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <unistd.h>

#define INITIAL_CAPACITY 32
#define GROWTH_FACTOR 2

typedef struct arrayhead_t {
  size_t lenght, capacity;
} arrayhead;

int *al_empty();

int *al_new(size_t lenght);

size_t al_lenght(const int array[]);

void al_insert(int array[], const int item);

void al_print(const int array[]);

#endif // !ARRAYLIST_H
