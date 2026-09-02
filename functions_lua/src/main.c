/*
 * main.c
 *
 * Load .lua files, store functions and run the main loop
 */
#include <dirent.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <math.h>
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

/* TODO: move to dedicated module */
static int c_pow(lua_State *L) {
  const double base = luaL_checknumber(L, 1), exp = luaL_checknumber(L, 2);
  lua_pushnumber(L, pow(base, exp));

  return 1;
}

static void print_functions() {
  for (int i = 0; i < funcs_size; i++)
    printf("%d. %s id=%d nparams=%d\n", i, lua_funcs[i].name, lua_funcs[i].ref,
           lua_funcs[i].nparams);
}

int main(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  // TODO: setup atexit()

  /*
   * Register C functions
   * TODO: refactor
   */
  lua_register(L, "c_pow", c_pow);

  /*
   * Load lua scripts
   */
  fprintf(stderr, "loading .lua files:\n");
  load_lua(L, LUA_DIR_PATH);

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
