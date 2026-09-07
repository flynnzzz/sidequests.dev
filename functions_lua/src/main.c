/*
 * main.c
 *
 * Startup the Lua Virtual Machine!
 */
#include "luautils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE                                                                  \
  " usage: luafun [OPTIONS] [function]\n\n"                                    \
  " OPTIONS:\n"                                                                \
  "   -h print this message\n"                                                 \
  "   -l list available functions\n"                                           \
  "   -x execute a function\n"

static int is_flag(const char *arg) { return arg[0] == '-'; }

int main(int argc, const char **argv) {

  if (argc <= 1) {
    printf(USAGE);
    exit(1);
  }

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaU_updatecpath(L);

  luaU_loadfns(L, LUA_DIRPATH);

  for (int i = 1; i < argc; i++) {

    if (is_flag(argv[i])) {

      if (strcmp(argv[i], "-h") == 0) {
        printf(USAGE);
        return 0;
      } else if (strcmp(argv[i], "-l") == 0) {
        printlua_fns();
        return 0;
      } else if (strcmp(argv[i], "-x")) {
        printf("WIP\n");
      }
    }
  }

  lua_close(L);
  return 0;
}
