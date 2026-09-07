/*
 * main.c
 *
 * Startup the Lua Virtual Machine.
 */
#include "luautils.h"
#include <stdio.h>

int main(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaU_updatecpath(L);

  luaU_loadfns(L, LUA_DIRPATH);

  const double res1 = luaU_dofunction(L, lua_funcs[0].ref, lua_funcs[0].nparams,
                                      2.0, 2.0),
               res2 = luaU_dofunction(L, lua_funcs[1].ref, lua_funcs[1].nparams,
                                      2.0);
  printf("executing functions:\n - %s -> %.2f\n - %s -> %.2f\n",
         lua_funcs[0].name, res1, lua_funcs[1].name, res2);

  lua_close(L);
  return 0;
}
