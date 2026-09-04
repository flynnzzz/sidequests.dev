/*
 * main.c
 *
 * Load .lua files, store functions and run the main loop
 */
#include <dirent.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUA_DIR_PATH "./lua"
#define LUA_EXCLUDE_PATH "cmath.lua"
#define join_path(base, top)                                                   \
  {                                                                            \
    strcat(path, "/");                                                         \
    strcat(path, top);                                                         \
  }
#define BUFFER_SIZE 256
#define MAX_NAME_LEN 128
#define MAX_FUNCTIONS_NUM 128

typedef struct lua_function {
  char name[MAX_NAME_LEN];
  luaL_Reg reg;
  int ref, nparams;
} lua_fn;

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

static int lua_fn_nparams(lua_State *L, const char *fn_name) {
  lua_Debug ar;

  lua_getglobal(L, fn_name);
  lua_getinfo(L, ">u", &ar);

  return ar.nparams;
}

static void store_lua_fn(lua_State *L, const char *fn_name, lua_fn funcs[],
                         int *size) {
  lua_getglobal(L, fn_name);
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  if (ref == -1) {
    fprintf(stderr, "function '%s' not found\n", fn_name);
    return;
  }

  strcpy(funcs[(*size)].name, fn_name);
  funcs[(*size)].ref = ref;
  funcs[(*size)].nparams = lua_fn_nparams(L, fn_name);

  (*size)++;
}

/*
 * Load and save functions from the specified directory
 * to `lua_fn` array
 */
static void load_lua_fns(lua_State *L, const char *lua_dir, lua_fn funcs[],
                         int *size) {
  DIR *luadir = opendir(LUA_DIR_PATH);
  if (luadir == NULL) {
    perror(LUA_DIR_PATH);

    lua_close(L);
    exit(1);
  }

  struct dirent *d_entry;
  while ((d_entry = readdir(luadir)) != NULL) {
    if (!endswith(d_entry->d_name, ".lua") ||
        strcmp(d_entry->d_name, LUA_EXCLUDE_PATH) == 0)
      continue;

    char path[BUFFER_SIZE];
    strcpy(path, lua_dir);

    join_path(path, d_entry->d_name);
    luaL_dofile(L, path);

    const size_t filename_len = strlen(d_entry->d_name);

    char fn_name[MAX_NAME_LEN];
    strcpy(fn_name, d_entry->d_name);
    fn_name[filename_len - 4] = '\0';

    store_lua_fn(L, fn_name, funcs, size);
  }
  closedir(luadir);
}

static float execute_lua_fn(lua_State *L, const char *fn_name, int nargs,
                            double args[]) {

  lua_getglobal(L, fn_name);
  if (!lua_isfunction(L, -1)) {
    fprintf(stderr, "'f' is not a Lua function\n");

    lua_close(L);
    exit(1);
  }

  for (int i = 0; i < nargs; i++) {
    lua_pushnumber(L, args[i]);
  }

  if (lua_pcall(L, nargs, 1, 0) != LUA_OK) {
    fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));

    lua_close(L);
    exit(1);
  }

  float result = lua_tonumber(L, -1);
  lua_pop(L, 1);

  return result;
}

static void update_cpath(lua_State *L) {
  fprintf(stderr, "updating cpath...\n");

  lua_getglobal(L, "package");
  lua_getfield(L, -1, "cpath");
  const char *current_cpath = lua_tostring(L, -1);

  char new_cpath[512];
  snprintf(new_cpath, sizeof(new_cpath), "./bin/?.so;%s", current_cpath);
  fprintf(stderr, "\nnew cpath set: %s\n\n", new_cpath);
  lua_pop(L, 1);

  lua_pushstring(L, new_cpath);
  lua_setfield(L, -2, "cpath");
  lua_pop(L, 1);
}

int main(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  update_cpath(L);

  lua_fn lua_funcs[MAX_FUNCTIONS_NUM];
  int funcs_size = 0;
  load_lua_fns(L, LUA_DIR_PATH, lua_funcs, &funcs_size);

  // TODO: replace with variable args
  double argsf[] = {2, 2}, argsg[] = {2};
  execute_lua_fn(L, "f", 2, argsf);
  execute_lua_fn(L, "g", 1, argsg);

  lua_close(L);
  return EXIT_SUCCESS;
}
