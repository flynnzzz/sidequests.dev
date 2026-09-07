/*
 * luautils.h
 *
 * Utils module to simplify loading Lua files.
 */
#ifndef LUAUTILS_H
#define LUAUTILS_H

#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <stdarg.h>

#define LUA_DIRPATH "./lua"
#define LUA_EXCLUDEFILE "cmath.lua"
#define MAX_NAME_LEN 128
#define MAX_FUNCTIONS_NUM 128

typedef struct lua_function {
  char name[MAX_NAME_LEN];
  int ref, nparams;
} lua_fn;

extern lua_fn lua_funcs[MAX_FUNCTIONS_NUM];
extern int nfuncs;

int luaU_fnparams(lua_State *L, int ref);

/*
 * Load and save functions from the specified directory
 * to `lua_fn` array.
 */
void luaU_loadfns(lua_State *L, const char *lua_dir);

/*
 * NOTE: passing variadic parameters of type `int` generates undefined
 * behaviour. It is recommended to pass in double or float types.
 */
double luaU_dofunction(lua_State *L, int ref, int nparams, ...);

/*
 * Updates package.cpath to include the binaries folder of the current program.
 */
void luaU_updatecpath(lua_State *L);

#endif // !LUAUTILS_H
