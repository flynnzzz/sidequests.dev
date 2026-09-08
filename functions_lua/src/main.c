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
  "   -l list available functions\n"                                           \
  "   -x execute a function\n"
#define BUFFER_SIZE 128
#define FLAG_LEN 2

static int to_arrayidx(int idx) { return idx - 1; }

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
      strcpy(flag, argv[i]);
      flag_found = 1;
    }
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
        strcpy(target, argv[i]);
        target_found = 1;
        target_index = i;
      }
    }

    int function_index = atoi(target);
    if (function_index == 0 ||
        (function_index = to_arrayidx(function_index)) >= nfuncs) {
      fprintf(stderr, "[ERROR] invalid target\n");
      lua_close(L);
      exit(1);
    }

    const int ref = luaU_globalfuntions[function_index].ref,
              nparams = luaU_globalfuntions[function_index].nparams;

    // -> function | ...
    luaU_rawgetfn(L, ref);

    const int provided_params = argc - 3;
    if (provided_params < nparams) {
      fprintf(stderr, "[ERROR] not enough arguments\n");
      lua_close(L);
      exit(1);
    }

    // -> p1 | -> ... | -> pn | function | ...
    for (int i = target_index; i < target_index + nparams; i++) {
      lua_pushnumber(L, atof(argv[i + 1]));
    }

    // <- result | ...
    printf("Result: %f\n", luaU_pcall(L, nparams, 1, 0));
    lua_close(L);

  } break;
  case 'l': {
    L = init_lua();
    printlua_fns();
    lua_close(L);
  } break;
  default: {
    printf(USAGE);
  } break;
  }

  return 0;
}
