/*
 * arraylist.h
 *
 * Dynamic array interface.
 *
 * This implementation makes use of the `hidden header` method by
 * storing metadata in the memory block right before the the first item.
 *
 * Hence, is strongly discouraged to use functions from this module for anything
 * but arraylists allocated with the provided constructors, or else `undefined
 * behaviour` will occur.
 *
 * NOTE: Not yet implemented:
 * The API Achieves generics like behaviour through the use of macros,
 * exchanging code length for simplicity and compile time type checking.
 *
 */

#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stddef.h>

#define AL_INIT_CAPACITY 32
#define AL_MAXSIZE ((~(size_t)0) >> 1)
#define AL_MAXCAP ((AL_MAXSIZE - sizeof(alH_head)) / sizeof(T))
#define AL_GROWTH_FACTOR 2

// NOTE: temporary
typedef int T;

enum alH_errstatus { AL_FAIL, AL_SUCCESS };
#define AL_ERRSIZE (~(size_t)0)
#define AL_ERRTYPE (~(T)0)

typedef struct alH_head_t {
  size_t length, capacity;
} alH_head;

/*
 * Allocates an empty arraylist of `AL_INIT_CAPACITY`.
 *
 * Returns `NULL` in case of failure.
 */
T *al_init();

/*
 * Allocates an empty arraylist of a specified `capacity`.
 *
 * Returns `NULL` in case of failure.
 */
T *al_create(size_t capacity);

/*
 * Frees an arraylist allocated with either `al_create()` or `al_init()`.
 *
 * It is strongly discouraged to use this function for anything but
 * "arraylists", as doing so will cause undefined behaviour.
 *
 * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.
 */
int al_free(T array[]);

/*
 * Returns the current length of an arraylist, `AL_ERRSIZE` in case of failure.
 */
size_t al_length(const T array[]);

/*
 * Returns the capacity of an arraylist, `AL_ERRSIZE` in case of failure.
 */
size_t al_capacity(const T array[]);

/*
 * Increases the capacity of an arraylist by `AL_GROWTH_FACTOR`.
 *
 * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.
 */
int al_grow(T *arrptr[]);

/*
 * Resizes the capacity of an arraylist by the specified amount.
 *
 * Shrinking the arraylist will permanently delete (if present) all items past
 * the new decrased capacity.
 *
 * Passing in 0 as `capacity` will set the new capacity to
 * `AL_INIT_CAPACITY`.
 *
 * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.
 */
int al_resize(T *arrptr[], size_t capacity);

/*
 * Appends the specified `item` to the tail of the arraylist.
 *
 * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.
 */
int al_push(T *arrptr[], const T item);

/*
 * Removes and returns last item in the arraylist.
 *
 * Returns `AL_ERRTYPE` in case of error.
 */
T al_pop(T array[]);

/*
 * Inserts the specified `item` at `index`, automatically shifting
 * all items.
 *
 * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.
 */
int al_insert(T *arrptr[], size_t index, const T item);

/*
 * Removes and returns the first instance of the specified `item` in the
 * arraylist.
 *
 * This function accepts a comparator callback, it can be set to `NULL` for
 * plain value comparison.
 *
 * Returns `AL_ERRTYPE` if no item was found or in case an error was
 * encountered.
 */
T al_rm(T array[], const T item, int (*cmp)(T a, T b));

/*
 * Removes all instances of the specified `item` in the arraylist.
 *
 * This function accepts a comparator callback, it can be set to `NULL` for
 * plain value comparison.
 *
 * Returns the number of removed items.
 */
int al_rmall(T array[], const T item, int (*cmp)(T a, T b));

/*
 * Removes and returns the item at the specified `index`.
 *
 * Returns `AL_ERRTYPE` if no item was found or in case an error was
 * encountered.
 */
T al_rmat(T array[], size_t index);

/*
 * Prints the contents of an arraylist, followed by a newline separator.
 *
 * Callback `int to_string(T item, char* buf)` should write the
 * string representation of an item (max size `AL_MAXBUF`) to the buffer `buf`.
 *
 * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.
 */
int al_print(const T array[], int (*to_string)(T, char *));

/*
 * Clears every item in the arraylist.
 *
 * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.
 */
int al_clear(T array[]);

/*
 * Returns the index if `item` was found.
 *
 * This function accepts a comparator callback, it can be set to `NULL` for
 * plain value comparison.
 *
 * Returns `AL_ERRSIZE` in case of failure.
 */
size_t al_contains(T array[], T item, int (*cmp)(T a, T b));

// TODO: implement
T *al_copy(T array[]);

// TODO: implement
int al_swap(T array[], size_t a, size_t b);

#endif // !ARRAYLIST_H
