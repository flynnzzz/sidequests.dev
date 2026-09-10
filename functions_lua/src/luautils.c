
/*
 * luautils.c
 *
 * Implementations of luautils.h.
 */
#include "luautils.h"
#include <dirent.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATHLEN 512
#define EXTENSION_NAME 4 // '.lua'
#define join_path(base, top)                                                   \
  {                                                                            \
    strncat(path, "/", 2);                                                     \
    strncat(path, top, MAX_PATHLEN - strlen(base));                            \
  }

/*
 * Auxiliary macro.
 *
 * Pushes `n` numbers of `type` onto the stack.
 * The last parameter preceiding the `n` variatic args needs to be
 * specified in `last`.
 */
#define luaU_pushvariadic(type, n, last)                                       \
  {                                                                            \
    va_list ap;                                                                \
    va_start(ap, last);                                                        \
                                                                               \
    for (int i = 0; i < n && i < MAX_FPARAM; i++) {                            \
      type arg = va_arg(ap, type);                                             \
      lua_pushnumber(L, arg);                                                  \
    }                                                                          \
                                                                               \
    va_end(ap);                                                                \
  }

const char letters[MAX_FPARAM] = "abcdefghijklmnopqrstuvwxyz";
luaU_fn luaU_globalfunctions[MAX_NFUNCTIONS];
int nfuncs = 0;

// Source - https://stackoverflow.com/a/744822
// Posted by plinth, modified by community. See post 'Timeline' for change
// history Retrieved 2026-08-27, License - CC BY-SA 2.5
static int endswith(const char *str, const char *suffix) {
  if (!str || !suffix)
    return 0;
  const size_t lenstr = strlen(str), lensuffix = strlen(suffix);
  if (lensuffix > lenstr)
    return 0;
  return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

static void luaU_storefn(lua_State *L, const char *fn_name) {
  // <- function | ...
  const int ref = luaL_ref(L, LUA_REGISTRYINDEX);

  if (ref == LUA_REFNIL) {
    fprintf(stderr, "[WARNING] function '%s' not found\n", fn_name);
    return;
  }

  // !global: lua_funcs, nfuncs
  strncpy(luaU_globalfunctions[nfuncs].name, fn_name, MAX_NAMELEN);
  luaU_globalfunctions[nfuncs].ref = ref;
  luaU_globalfunctions[nfuncs].nparams = luaU_fnparams(L, ref);
  nfuncs++;
}

int luaU_fnparams(lua_State *L, int ref) {
  lua_Debug ar;

  // -> function | ...
  lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
  if (!lua_isfunction(L, -1)) {
    fprintf(stderr,
            " [ERROR] id '%d' does not refer to a Lua function, unable to "
            "retreive info.\n",
            ref);
    lua_pop(L, 1);
    return -1;
  }
  // <- function | ...
  lua_getinfo(L, ">u", &ar);

  return ar.nparams;
}

void luaU_loadfns(lua_State *L, const char *lua_dir) {
  DIR *luadir = opendir(LUA_DIRPATH);
  if (luadir == NULL) {
    fprintf(stderr, " [ERROR] could not open %s.\n", LUA_DIRPATH);
    lua_close(L);
    exit(1);
  }

  struct dirent *d_entry;
  while ((d_entry = readdir(luadir)) != NULL) {
    if (!endswith(d_entry->d_name, ".lua") ||
        strncmp(d_entry->d_name, LUA_EXCLUDEFILE, MAX_NAMELEN) == 0)
      continue;

    char path[MAX_PATHLEN];
    strncpy(path, lua_dir, MAX_PATHLEN);
    join_path(path, d_entry->d_name);

    // -> function | ...
    if (luaL_loadfile(L, path) != LUA_OK || lua_pcall(L, 0, 1, 0) != LUA_OK ||
        !lua_isfunction(L, -1)) {
      fprintf(stderr, " [ERROR] %s\n", lua_tostring(L, -1));
      lua_pop(L, 1);
      continue;
    }

    char fn_name[MAX_NAMELEN];
    strncpy(fn_name, d_entry->d_name, MAX_NAMELEN);

    /* truncate the .lua extension */
    fn_name[strlen(d_entry->d_name) - EXTENSION_NAME] = '\0';

    // <- function | ...
    luaU_storefn(L, fn_name);
  }
  closedir(luadir);
}

int luaU_rawgetfn(lua_State *L, int ref) {

  lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

  if (!lua_isfunction(L, -1)) {
    fprintf(stderr,
            " [ERROR] %d does not refer a Lua function, aborting execution.\n",
            ref);
    lua_pop(L, 1);
    return 1;
  }
  return 0;
}

double luaU_pcall(lua_State *L, int n, int r, int f) {

  // <- [p1, ..., pn, function], -> result | ...
  if (lua_pcall(L, n, r, f) != LUA_OK) {
    fprintf(stderr, " [ERROR] %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }

  const double result = lua_tonumber(L, -1);

  // <- result | ...
  lua_pop(L, 1);

  return result;
}

double luaU_dofunction(lua_State *L, int ref, int nparams, ...) {

  // -> function | ...
  luaU_rawgetfn(L, ref);

  // -> p1 | -> ... | -> pn | function | ...
  luaU_pushvariadic(double, nparams, nparams);

  // <- [p1, ..., pn, function, result] | ...
  return luaU_pcall(L, nparams, 1, 0);
}

double luaU_doluaufn(lua_State *L, int idx, ...) {

  const int ref = luaU_globalfunctions[idx].ref,
            nparams = luaU_globalfunctions[idx].nparams;

  /*
   * `luaU_dofunction`
   */
  luaU_rawgetfn(L, ref);
  luaU_pushvariadic(double, nparams, idx);
  return luaU_pcall(L, nparams, 1, 0);
}

void luaU_updatecpath(lua_State *L) {

  // -> cpath | -> package | ...
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "cpath");

  const char *current_cpath = lua_tostring(L, -1);
  char new_cpath[MAX_PATHLEN];
  snprintf(new_cpath, sizeof(new_cpath), "./bin/?.so;%s", current_cpath);
  // fprintf(stderr, "[INFO] new cpath set: \n'%s'\n", new_cpath);

  // <- cpath, -> new_cpath | package | ...
  lua_pop(L, 1);
  lua_pushstring(L, new_cpath);

  // <- new_cpath | <- package | ...
  lua_setfield(L, -2, "cpath");
  lua_pop(L, 1);
}

void printlua_fns() {
  for (int i = 0; i < nfuncs; i++) {
    printf(" %d. %s(", i + 1, luaU_globalfunctions[i].name);
    for (int j = 0; j < luaU_globalfunctions[i].nparams && j < strlen(letters);
         j++) {
      const char *format =
          j < luaU_globalfunctions[i].nparams - 1 && j < strlen(letters) - 1
              ? "%c, "
              : "%c";
      printf(format, letters[j]);
    }
    puts(")");
  }
}
