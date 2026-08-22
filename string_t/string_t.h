/*
 * string_t.h
 *
 * Custom string adt, created for learning purposes.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string_type {
  const char *ptr;
  size_t len;
};
typedef struct string_type *string_t;

/*
 * Heap-allocation
 */
string_t new_string(const char *c_string);

size_t stringlen(string_t string);

string_t stringcpy(string_t string);

string_t stringcat(string_t dest, string_t src);

string_t string_replace(string_t src, string_t old, string_t new);

/*
 * Appends a newline after printing `string`
 */
void println(string_t string);
