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

  double res1 = luaU_doluaufn(L, 0, 2.0, 2.0), res2 = luaU_doluaufn(L, 1, 2.0);

  printf("executing functions:\n - %s -> %.2f\n - %s -> %.2f\n",
         luaU_globalfuntions[0].name, res1, luaU_globalfuntions[1].name, res2);

  lua_close(L);
  return 0;
}
