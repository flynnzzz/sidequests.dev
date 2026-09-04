/*
 * main.c
 *
 * Load .lua files, store functions and run the main loop
 */
#include "luautils.h"
#define MAX_FUNCTIONS_NUM 128

int main(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  update_cpath(L);

  lua_fn lua_funcs[MAX_FUNCTIONS_NUM];
  int funcs_size = 0;
  load_lua_fns(L, LUA_DIR_PATH, lua_funcs, &funcs_size);

  // TODO: replace with variable args
  double argsf[] = {2, 2}, argsg[] = {2};
  printf("executing functions:\n - f -> %.2f\n - g -> %.2f\n",
         execute_lua_fn(L, "f", 2, argsf), execute_lua_fn(L, "g", 1, argsg));

  lua_close(L);
  return 0;
}
