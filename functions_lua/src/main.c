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

static lua_fn lua_funcs[MAX_FUNCTIONS_NUM];
static int funcs_size = 0;

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

static void store_lua_fn(lua_State *L, const char *fn_name) {
  lua_getglobal(L, fn_name);
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  if (ref == -1) {
    fprintf(stderr, "function '%s' not found\n", fn_name);
    return;
  }

  strcpy(lua_funcs[funcs_size].name, fn_name);
  lua_funcs[funcs_size].ref = ref;
  lua_funcs[funcs_size].nparams = lua_fn_nparams(L, fn_name);

  funcs_size++;
}

static void load_lua(lua_State *L, const char *lua_dir) {
  DIR *luadir = opendir(LUA_DIR_PATH);
  if (luadir == NULL) {
    perror(LUA_DIR_PATH);
    exit(1);
  }

  struct dirent *d_entry;
  while ((d_entry = readdir(luadir)) != NULL) {
    if (!endswith(d_entry->d_name, ".lua"))
      continue;

    char path[BUFFER_SIZE];
    strcpy(path, lua_dir);

    join_path(path, d_entry->d_name);
    luaL_dofile(L, path);

    const size_t filename_len = strlen(d_entry->d_name);

    char fn_name[MAX_NAME_LEN];
    strcpy(fn_name, d_entry->d_name);
    fn_name[filename_len - 4] = '\0';

    store_lua_fn(L, fn_name);
  }
  closedir(luadir);
}

static void print_functions() {
  for (int i = 0; i < funcs_size; i++)
    printf("%d. %s id=%d nparams=%d\n", i, lua_funcs[i].name, lua_funcs[i].ref,
           lua_funcs[i].nparams);
}

static void execute_lua_fn(lua_State *L, const char *fn_name, int nargs,
                           double args[]) {

  lua_getglobal(L, fn_name);
  if (!lua_isfunction(L, -1)) {
    fprintf(stderr, "'f' is not a Lua function\n");
    exit(1);
  }

  for (int i = 0; i < nargs; i++) {
    lua_pushnumber(L, args[i]);
  }

  if (lua_pcall(L, nargs, 1, 0) != LUA_OK) {
    fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
    exit(1);
  }

  printf("%.3f\n", lua_tonumber(L, -1));

  lua_pop(L, 1);
}

int main(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  /*
   * Set cpath
   */
  printf("updating cpath...\n");
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "cpath");
  const char *current_cpath = lua_tostring(L, -1);
  char new_cpath[512];
  snprintf(new_cpath, sizeof(new_cpath), "./bin/?.so;%s", current_cpath);
  fprintf(stderr, "new cpath: set %s\n", new_cpath);
  lua_pop(L, 1);
  lua_pushstring(L, new_cpath);
  lua_setfield(L, -2, "cpath");
  lua_pop(L, 1);

  // TODO: setup atexit()

  /*
   * Load lua scripts
   */
  fprintf(stderr, "loading .lua files:\n");
  load_lua(L, LUA_DIR_PATH);

  // TODO: replace with variable args
  double argsf[] = {2, 2}, argsg[] = {2};

  printf("testing 'f':\n");
  execute_lua_fn(L, "f", 2, argsf);
  printf("testing 'g':\n");
  execute_lua_fn(L, "g", 1, argsg);

  printf("WIP\n");
  int index;
  do {
    printf("Select function to execute:\n");
    print_functions();
    if (scanf("%d", &index) != 1)
      exit(0);

    // TODO: modify
    putchar(index);

  } while (1);

  lua_close(L);
  return EXIT_SUCCESS;
}
