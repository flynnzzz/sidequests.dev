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
#define MAX_NAMELEN 128
#define MAX_NFUNCTIONS 128

typedef struct luaU_function {
  char name[MAX_NAMELEN];
  int ref, nparams;
} luaU_fn;

extern luaU_fn luaU_globalfuntions[MAX_NFUNCTIONS];
extern int nfuncs;

int luaU_fnparams(lua_State *L, int ref);

/*
 * Load and save functions from the specified directory
 * to `lua_fn` array.
 */
void luaU_loadfns(lua_State *L, const char *lua_dir);

// TODO: add descriptions
int luaU_rawgetfn(lua_State *L, int ref);

double luaU_pcall(lua_State *L, int n, int r, int f);

/*
 * NOTE: passing variadic parameters of type `int` generates undefined
 * behaviour. It is recommended to pass in double or float types.
 */
double luaU_dofunction(lua_State *L, int ref, int nparams, ...);

double luaU_doluaufn(lua_State *L, int idx, ...);

/*
 * Updates package.cpath to include the binaries folder of the current program.
 */
void luaU_updatecpath(lua_State *L);

void printlua_fns();

#endif // !LUAUTILS_H
