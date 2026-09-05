/*
 * main.c
 *
 * Startup the Lua Virtual Machine.
 */
#include "luautils.h"
#define MAX_FUNCTIONS_NUM 128

int main(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  update_cpath(L);

  lua_fn lua_funcs[MAX_FUNCTIONS_NUM];
  load_lua_fns(L, LUA_DIR_PATH, lua_funcs);

  printf("executing functions:\n - f -> %.2f\n - g -> %.2f\n",
         execute_lua_fn(L, lua_funcs[0].name, lua_funcs[0].nparams, 2.0, 2.0),
         execute_lua_fn(L, lua_funcs[1].name, lua_funcs[1].nparams, 2.0));

  lua_close(L);
  return 0;
}
