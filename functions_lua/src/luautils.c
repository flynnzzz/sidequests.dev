
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

#define MAX_PATH_LEN 512
#define EXTENSION_NAME 4 // .lua
#define join_path(base, top)                                                   \
  {                                                                            \
    strcat(path, "/");                                                         \
    strcat(path, top);                                                         \
  }

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

static void store_lua_fn(lua_State *L, const char *fn_name, lua_fn funcs[],
                         int index) {
  // <- function | ...
  const int ref = luaL_ref(L, LUA_REGISTRYINDEX);

  if (ref == LUA_REFNIL) {
    fprintf(stderr, "[WARNING] function '%s' not found\n", fn_name);
    return;
  }

  strcpy(funcs[(index)].name, fn_name);
  funcs[index].ref = ref;
  funcs[index].nparams = lua_fn_nparams(L, ref);
}

int lua_fn_nparams(lua_State *L, int ref) {
  lua_Debug ar;

  // -> function | ...
  lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
  if (!lua_isfunction(L, -1)) {
    fprintf(stderr,
            "[ERROR] %d does not refer to a Lua function, unable to retreive "
            "info\n",
            ref);
    lua_pop(L, 1);
    return -1;
  }
  // <- function | ...
  lua_getinfo(L, ">u", &ar);

  return ar.nparams;
}

void load_lua_fns(lua_State *L, const char *lua_dir, lua_fn funcs[]) {
  DIR *luadir = opendir(LUA_DIR_PATH);
  if (luadir == NULL) {
    fprintf(stderr, "[ERROR] could not open %s\n", LUA_DIR_PATH);
    lua_close(L);
    exit(1);
  }

  struct dirent *d_entry;
  for (int i = 0; (d_entry = readdir(luadir)) != NULL;) {
    if (!endswith(d_entry->d_name, ".lua") ||
        strcmp(d_entry->d_name, LUA_EXCLUDEFILE) == 0)
      continue;

    char path[MAX_PATH_LEN];
    strcpy(path, lua_dir);
    join_path(path, d_entry->d_name);

    // -> function | ...
    if (luaL_loadfile(L, path) != LUA_OK || lua_pcall(L, 0, 1, 0) != LUA_OK ||
        !lua_isfunction(L, -1)) {
      fprintf(stderr, "[ERROR] %s\n", lua_tostring(L, -1));
      lua_pop(L, 1);
      continue;
    }

    char fn_name[MAX_NAME_LEN];
    strcpy(fn_name, d_entry->d_name);

    /* truncate the .lua extension */
    fn_name[strlen(d_entry->d_name) - EXTENSION_NAME] = '\0';

    // <- function | ...
    store_lua_fn(L, fn_name, funcs, i++);
  }
  closedir(luadir);
}

double execute_lua_fn(lua_State *L, int ref, int nparams, ...) {

  // -> function | ...
  lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

  if (!lua_isfunction(L, -1)) {
    fprintf(stderr,
            "[ERROR] %d does not refer a Lua function, aborting execution\n",
            ref);
    lua_pop(L, 1);
    return 0;
  }

  va_list ap;
  va_start(ap, nparams);
  for (int i = 0; i < nparams; i++) {
    double arg = va_arg(ap, double);
    lua_pushnumber(L, arg);
  }
  // -> p1 | -> ... | -> pn | function | ...

  va_end(ap);

  // <- [p1, ..., pn, function], -> result | ...
  if (lua_pcall(L, nparams, 1, 0) != LUA_OK) {
    fprintf(stderr, "[ERROR] %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }

  const double result = lua_tonumber(L, -1);

  // <- result | ...
  lua_pop(L, 1);

  return result;
}

void update_cpath(lua_State *L) {

  // -> cpath | -> package | ...
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "cpath");

  const char *current_cpath = lua_tostring(L, -1);
  char new_cpath[512];
  snprintf(new_cpath, sizeof(new_cpath), "./bin/?.so;%s", current_cpath);
  fprintf(stderr, "[INFO] new cpath set: \n'%s'\n", new_cpath);

  // <- cpath, -> new_cpath | package | ...
  lua_pop(L, 1);
  lua_pushstring(L, new_cpath);

  // <- new_cpath | <- package | ...
  lua_setfield(L, -2, "cpath");
  lua_pop(L, 1);
}
