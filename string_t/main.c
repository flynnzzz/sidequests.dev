/*
 * main.c
 *
 * Simple string replace tool utilizing
 * my string_t data structure implementation
 */

#include "string_t.h"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    println(new_string("usage: ./string_replace <string> <old> <new>"));
    exit(1);
  }
  const string_t src = new_string(argv[1]), old = new_string(argv[2]),
                 new = new_string(argv[3]);

  println(string_replace(src, old, new));

  free(src);
  free(new);
  return EXIT_SUCCESS;
}
