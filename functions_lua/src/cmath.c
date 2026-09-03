#include "cmath.h"

int c_pow(lua_State *L) {
  const double base = luaL_checknumber(L, 1), exp = luaL_checknumber(L, 2);
  lua_pushnumber(L, pow(base, exp));

  return 1;
}
