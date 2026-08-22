/*
 * string_t.c
 *
 * Implementation of string_t.h
 */

#define MAX_OCCURENCES 1024
#define MALLOC_ERROR -64
#define MALLOC_ERROR_MSG "Memory allocation error\n"

#include "string_t.h"
#include <stdint.h>
#include <string.h>

string_t new_string(const char *c_string) {
  string_t res = (string_t)malloc(sizeof(struct string_type));
  if (res == NULL)
    exit(MALLOC_ERROR);
  res->ptr = c_string;
  res->len = strlen(c_string);
  return res;
}

size_t stringlen(string_t string) { return string->len; }

string_t stringcpy(string_t string) {
  char *cpyptr = (char *)malloc(strlen(string->ptr) + 1);
  if (cpyptr == NULL)
    exit(MALLOC_ERROR);
  strcpy(cpyptr, string->ptr);
  return new_string(cpyptr);
}

string_t stringcat(string_t dest, string_t src) {
  char *newptr = (char *)malloc(strlen(dest->ptr) + strlen(src->ptr) + 1);
  if (newptr == NULL) {
    fprintf(stderr, MALLOC_ERROR_MSG);
    exit(MALLOC_ERROR);
  }
  strcat(newptr, dest->ptr);

  strcat(newptr, src->ptr);
  return new_string(newptr);
}

string_t string_replace(string_t src, string_t old, string_t new) {

  if (!stringlen(src) | !stringlen(old) | !stringlen(new))
    return stringcpy(src);

  char *matches[MAX_OCCURENCES];
  uint64_t occurences = 0;

  char *match;
  char *haysack = (char *)src->ptr;
  while ((match = strstr(haysack, old->ptr)) != NULL) {
    matches[occurences] = match;

    const size_t dist = (size_t)(match - haysack);
    haysack += dist + stringlen(old);
    occurences++;
  }

  if (occurences == 0)
    return stringcpy(src);

  const size_t finalsize =
      stringlen(src) + (stringlen(new) - stringlen(old)) * occurences + 1;

  char *res = (char *)malloc(finalsize);
  if (res == NULL) {
    fprintf(stderr, MALLOC_ERROR_MSG);
    exit(MALLOC_ERROR);
  }

  const char *start = src->ptr;
  char *newstring = res;
  const char *found;
  for (int i = 0; i < occurences; i++) {
    found = matches[i];

    size_t dist = (size_t)(found - start);
    memcpy(newstring, start, dist);
    newstring += dist;
    memcpy(newstring, new->ptr, stringlen(new));
    newstring += stringlen(new);
    start = found + stringlen(old);
  }
  found += stringlen(old);
  memcpy(newstring, found, strlen(found));

  newstring[finalsize] = '\0';
  return new_string(res);
}

void println(string_t string) { printf("%s\n", string->ptr); }
