/*
 * main.c
 *
 * Startup the Lua Virtual Machine.
 */
#include "luautils.h"
#include <stdio.h>
#define MAX_FUNCTIONS_NUM 128

int main(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  update_cpath(L);

  lua_fn lua_funcs[MAX_FUNCTIONS_NUM];
  load_lua_fns(L, LUA_DIR_PATH, lua_funcs);

  const double res1 = execute_lua_fn(L, lua_funcs[0].name, lua_funcs[0].nparams,
                                     2.0),
               res2 = execute_lua_fn(L, lua_funcs[1].name, lua_funcs[1].nparams,
                                     2.0, 2.0);
  printf("executing functions:\n - %s -> %.2f\n - %s -> %.2f\n",
         lua_funcs[0].name, res1, lua_funcs[1].name, res2);

  lua_close(L);
  return 0;
}
