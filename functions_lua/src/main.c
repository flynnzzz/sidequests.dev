/*
 * main.c
 *
 * Startup the Lua Virtual Machine!
 */
#include "luautils.h"
#include <lua5.4/lua.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE                                                                  \
  " usage: luafun [OPTIONS] [function]\n\n"                                    \
  " OPTIONS:\n"                                                                \
  "   -h print this message\n"                                                 \
  "   -l list all available functions\n"                                       \
  "   -x execute a function\n"
#define USE_USAGE " Try luafun -h for more information."
#define LIST_USAGE " Try luafun -l to list available functions."
#define BUFFER_SIZE 128
#define FLAG_LEN 2 + 1

static int is_flag(const char *arg) { return arg[0] == '-'; }

static lua_State *init_lua() {

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaU_updatecpath(L);
  luaU_loadfns(L, LUA_DIRPATH);

  return L;
}

int main(int argc, const char **argv) {

  if (argc <= 1) {
    printf(USAGE);
    exit(1);
  }

  char flag[FLAG_LEN];
  int flag_found = 0;
  for (int i = 0; i < argc && !flag_found; i++) {
    if (is_flag(argv[i])) {
      strncpy(flag, argv[i], FLAG_LEN);
      flag[FLAG_LEN - 1] = '\0';
      flag_found = 1;
    }
  }

  if (!flag_found) {
    puts(USE_USAGE);
    exit(1);
  }

  lua_State *L;
  char flagchar = flag[1];
  switch (flagchar) {
  case 'x': {
    L = init_lua();

    char target[BUFFER_SIZE];
    int target_found = 0, target_index;
    for (int i = 1; i < argc && !target_found; i++) {
      if (!is_flag(argv[i])) {
        strncpy(target, argv[i], BUFFER_SIZE);
        target[BUFFER_SIZE - 1] = '\0';
        target_found = 1;
        target_index = i;
      }
    }

    if (!target_found) {
      puts(" no target specified.\n");
      puts(USE_USAGE);
      exit(1);
    }

    int function_index = atoi(target);
    if (function_index == 0 || --function_index >= nfuncs) {
      puts(" invalid target index.\n");
      puts(LIST_USAGE);
      lua_close(L);
      exit(1);
    }

    const int ref = luaU_globalfunctions[function_index].ref,
              nparams = luaU_globalfunctions[function_index].nparams;
    const char *function_name = luaU_globalfunctions[function_index].name;

    // -> function | ...
    luaU_rawgetfn(L, ref);

    const int provided_params = argc - 3;
    if (provided_params < nparams) {
      fprintf(stderr,
              " [ERROR] not enough arguments for '%s': %d required but "
              "%d were given.\n",
              function_name, nparams, provided_params);
      lua_close(L);
      exit(1);
    }

    // -> p1 | -> ... | -> pn | function | ...
    const int endarg = target_index + nparams;
    for (int i = target_index; i < endarg; i++) {
      lua_pushnumber(L, atof(argv[i + 1]));
    }

    // <- result | ...
    const double result = luaU_pcall(L, nparams, 1, 0);

    printf(" %s(", function_name);
    for (int i = target_index; i < endarg; i++) {
      const char *format = i < endarg - 1 ? "%.2f, " : "%.2f";
      printf(format, atof(argv[i + 1]));
    }
    printf(") -> %.2f\n", result);

    lua_close(L);

  } break;
  case 'l': {
    L = init_lua();
    printlua_fns();
    lua_close(L);
  } break;
  case 'h': {
    puts(USAGE);
  } break;
  default: {
    printf(" unrecognized flag '-%c'\n\n", flagchar);
    puts(USE_USAGE);
  } break;
  }

  return 0;
}
