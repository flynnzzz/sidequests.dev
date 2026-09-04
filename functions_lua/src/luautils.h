/*
 * luautils.h
 *
 * Utils module to simplify loading Lua files.
 * By convention every Lua file should contain at least one single function
 * which shares names with the source file, this ensures that it will be loaded.
 *  TODO: this restriction will be lifted shortly
 */
#ifndef LUAUTILS_H
#define LUAUTILS_H

#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>

#define LUA_DIR_PATH "./lua"
#define LUA_EXCLUDE_PATH "cmath.lua"

#define MAX_NAME_LEN 128

typedef struct lua_function {
  char name[MAX_NAME_LEN];
  luaL_Reg reg;
  int ref, nparams;
} lua_fn;

int lua_fn_nparams(lua_State *L, const char *fn_name);

/*
 * Load and save functions from the specified directory
 * to `lua_fn` array
 */
void load_lua_fns(lua_State *L, const char *lua_dir, lua_fn funcs[]);

float execute_lua_fn(lua_State *L, const char *fn_name, int nargs,
                     double args[]);

void update_cpath(lua_State *L);

#endif // !LUAUTILS_H
