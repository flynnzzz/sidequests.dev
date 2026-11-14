/*
 * arraylist.h
 *
 * Dynamic array interface.
 *
 * This implementation makes use of the `hidden header` method by
 * storing metadata right before the start of the first item.
 *
 * Hence is strongly discouraged to use functions from this module for anything
 * but arraylist allocated with the provided constructors, or else `undefined
 * behaviour` occurs.
 *
 * NOTE: Not yet implemented:
 * The API Achieves generics like behaviour through the use of macros,
 * exchanging code length for simplicity and compile time type checking.
 *
 */

#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stddef.h>

#define INITIAL_CAPACITY 32
#define MAX_SIZE ((~(size_t)0) >> 1)
#define MAX_CAPACITY ((MAX_SIZE - sizeof(alH_head)) / sizeof(al_type))
#define GROWTH_FACTOR 2

#define AL_ERRSIZE (~(size_t)0)
#define AL_ERRTYPE (~(al_type)0)

enum alH_errstatus { AL_FAIL, AL_SUCCESS };

// NOTE: temporary
typedef int al_type;

typedef struct alH_head_t {
  size_t length, capacity;
} alH_head;

/*
 * Allocates an empty arraylist of `INITIAL_CAPACITY`.
 */
al_type *al_empty();

/*
 * Allocates an empty arraylist of a specified `capacity`.
 */
al_type *al_create(size_t capacity);

/*
 * Frees an arraylist allocated with either `al_create()` or `al_empty()`.
 *
 * It is strongly discouraged to use this function for anything but arraylists
 * since it produces undefined behaviour.
 */
int al_free(al_type array[]);

/*
 * Returns the current length of an arraylist.
 */
size_t al_length(const al_type array[]);

/*
 * Returns the capacity of an arraylist.
 */
size_t al_capacity(const al_type array[]);

/*
 * Appends the specified `item` to the tail of the arraylist.
 */
int al_push(al_type *arrayptr[], const al_type item);

/*
 * Removes and returns last item in the arraylist.
 *
 * Returns `AL_ERRTYPE` in case of error.
 */
al_type al_pop(al_type array[]);

/*
 * Inserts the specified `item` to the specified `index`.
 */
int al_insert(al_type *arrayptr[], size_t index, al_type item);

/*
 * Removes and returns the first instance of the specified `item`` in the
 * arraylist.
 *
 * Returns `AL_ERRTYPE` if no item was found or in case an error was
 * encountered.
 */
al_type al_rmfirst(al_type array[], const al_type item);

/*
 * Removes all instances of the specified `item`` in the arraylist.
 *
 * Returns `AL_SUCCESS` or `AL_FAIL` depending on the success of the operation.
 */
int al_rmall(al_type array[], const al_type item);

/*
 * Removes and returns the item at the specified `index`.
 *
 * Returns `AL_ERRTYPE` if no item was found or in case an error was
 * encountered.
 */
al_type al_rmat(al_type array[], size_t index);

/*
 * Prints the contents of an arraylist, followed by a newline separator.
 */
int al_print(const al_type array[]);

// TODO: implement
int al_clear(al_type array[]);

// TODO: implement
int al_fclear(al_type array[]);

// TODO: implement
al_type *al_copy(al_type array[]);

// TODO: implement
int al_resize(al_type array[], size_t capacity);

// TODO: implement
int al_grow(al_type array[]);

// TODO: implement
int al_swap(al_type array[], size_t index1, size_t index2);

// TODO: implement
int al_sort(al_type array[], int (*comparator)(al_type, al_type));

// TODO: implement
int al_reverse(al_type array[]);

#endif // !ARRAYLIST_H
