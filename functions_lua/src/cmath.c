#include "cmath.h"

int c_pow(lua_State *L) {
  const double base = luaL_checknumber(L, 1), exp = luaL_checknumber(L, 2);
  lua_pushnumber(L, pow(base, exp));

  return 1;
}

/*
 * Register C functions
 */

static const luaL_Reg module[] = {{"pow", c_pow}, {NULL, NULL}};

int luaopen_cmath(lua_State *L) {
  luaL_newlib(L, module);
  return 1;
}
