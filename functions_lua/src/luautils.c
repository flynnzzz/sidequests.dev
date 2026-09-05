
/*
 * luautils.c
 *
 * TODO: handle unhandled errors
 */
#include "luautils.h"
#include <dirent.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
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
  lua_getglobal(L, fn_name);
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  if (ref == -1) {
    fprintf(stderr, "function '%s' not found\n", fn_name);
    return;
  }

  strcpy(funcs[(index)].name, fn_name);
  funcs[(index)].ref = ref;
  funcs[(index)].nparams = lua_fn_nparams(L, fn_name);
}

int lua_fn_nparams(lua_State *L, const char *fn_name) {
  lua_Debug ar;

  lua_getglobal(L, fn_name);
  lua_getinfo(L, ">u", &ar);

  return ar.nparams;
}

void load_lua_fns(lua_State *L, const char *lua_dir, lua_fn funcs[]) {
  DIR *luadir = opendir(LUA_DIR_PATH);
  if (luadir == NULL) {
    perror(LUA_DIR_PATH);
    lua_close(L);
    exit(1);
  }

  struct dirent *d_entry;
  for (int i = 0; (d_entry = readdir(luadir)) != NULL; i++) {
    if (!endswith(d_entry->d_name, ".lua") ||
        strcmp(d_entry->d_name, LUA_EXCLUDE_PATH) == 0)
      continue;

    char path[BUFFER_SIZE];
    strcpy(path, lua_dir);

    join_path(path, d_entry->d_name);
    if (luaL_dofile(L, path) != LUA_OK) {
      perror(lua_tostring(L, -1));
      lua_pop(L, 1);
      continue;
    }

    const size_t name_len = strlen(d_entry->d_name);
    char fn_name[MAX_NAME_LEN];
    strcpy(fn_name, d_entry->d_name);

    /* truncate the .lua extension */
    fn_name[name_len - 4] = '\0';

    store_lua_fn(L, fn_name, funcs, i);
  }
  closedir(luadir);
}

double execute_lua_fn(lua_State *L, const char *fn_name, int nparams, ...) {

  lua_getglobal(L, fn_name);
  if (!lua_isfunction(L, -1)) {
    fprintf(stderr, "'f' is not a Lua function\n");
    lua_close(L);
    exit(1);
  }

  va_list ap;
  va_start(ap, nparams);
  for (int i = 0; i < nparams; i++) {
    lua_pushnumber(L, va_arg(ap, double));
  }
  va_end(ap);

  if (lua_pcall(L, nparams, 1, 0) != LUA_OK) {
    perror(lua_tostring(L, -1));
    lua_close(L);
    exit(1);
  }

  const double result = lua_tonumber(L, -1);
  lua_pop(L, 1);

  return result;
}

void update_cpath(lua_State *L) {
  fprintf(stderr, "updating cpath...\n");

  lua_getglobal(L, "package");
  lua_getfield(L, -1, "cpath");
  const char *current_cpath = lua_tostring(L, -1);

  char new_cpath[512];
  snprintf(new_cpath, sizeof(new_cpath), "./bin/?.so;%s", current_cpath);
  fprintf(stderr, "new cpath set: %s\n", new_cpath);
  lua_pop(L, 1);

  lua_pushstring(L, new_cpath);
  lua_setfield(L, -2, "cpath");
  lua_pop(L, 1);
}
