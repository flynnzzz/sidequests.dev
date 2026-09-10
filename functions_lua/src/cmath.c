/*
 * cmath.c
 *
 * Exposed C functions for Lua
 */
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <math.h>

static int c_add(lua_State *L) {
  const double a = luaL_checknumber(L, 1), b = luaL_checknumber(L, 2);
  lua_pushnumber(L, a + b);

  return 1;
}

static int c_sub(lua_State *L) {
  const double a = luaL_checknumber(L, 1), b = luaL_checknumber(L, 2);
  lua_pushnumber(L, a - b);

  return 1;
}

static int c_mul(lua_State *L) {
  const double a = luaL_checknumber(L, 1), b = luaL_checknumber(L, 2);
  lua_pushnumber(L, a * b);

  return 1;
}

static int c_div(lua_State *L) {
  const double a = luaL_checknumber(L, 1), b = luaL_checknumber(L, 2);
  if (b == 0.0) {
    return luaL_error(L, "division by zero");
  }
  lua_pushnumber(L, a / b);

  return 1;
}

static int c_pow(lua_State *L) {
  const double base = luaL_checknumber(L, 1), exp = luaL_checknumber(L, 2);
  lua_pushnumber(L, pow(base, exp));

  return 1;
}

static int c_sqrt(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, sqrt(x));

  return 1;
}

static int c_abs(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, fabs(x));

  return 1;
}

static int c_sin(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, sin(x));

  return 1;
}

static int c_cos(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, cos(x));

  return 1;
}

static int c_tan(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, tan(x));

  return 1;
}

static int c_asin(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, asin(x));

  return 1;
}

static int c_acos(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, acos(x));

  return 1;
}

static int c_atan(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, atan(x));

  return 1;
}

static int c_atan2(lua_State *L) {
  const double y = luaL_checknumber(L, 1), x = luaL_checknumber(L, 2);
  lua_pushnumber(L, atan2(y, x));

  return 1;
}

static int c_exp(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, exp(x));

  return 1;
}

static int c_log(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, log(x));

  return 1;
}

static int c_log10(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, log10(x));

  return 1;
}

static int c_ceil(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, ceil(x));

  return 1;
}

static int c_floor(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, floor(x));

  return 1;
}

static int c_fmod(lua_State *L) {
  const double x = luaL_checknumber(L, 1), y = luaL_checknumber(L, 2);
  lua_pushnumber(L, fmod(x, y));

  return 1;
}

static int c_min(lua_State *L) {
  const double a = luaL_checknumber(L, 1), b = luaL_checknumber(L, 2);
  lua_pushnumber(L, a < b ? a : b);

  return 1;
}

static int c_max(lua_State *L) {
  const double a = luaL_checknumber(L, 1), b = luaL_checknumber(L, 2);
  lua_pushnumber(L, a > b ? a : b);

  return 1;
}

static int c_deg(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, x * 180.0 / M_PI);

  return 1;
}

static int c_rad(lua_State *L) {
  const double x = luaL_checknumber(L, 1);
  lua_pushnumber(L, x * M_PI / 180.0);

  return 1;
}

/*
 * Register C functions
 */
static const luaL_Reg module[] = {
    {"add", c_add},     {"sub", c_sub},     {"mul", c_mul},
    {"div", c_div},     {"pow", c_pow},     {"sqrt", c_sqrt},
    {"abs", c_abs},     {"sin", c_sin},     {"cos", c_cos},
    {"tan", c_tan},     {"asin", c_asin},   {"acos", c_acos},
    {"atan", c_atan},   {"atan2", c_atan2}, {"exp", c_exp},
    {"log", c_log},     {"log10", c_log10}, {"ceil", c_ceil},
    {"floor", c_floor}, {"fmod", c_fmod},   {"min", c_min},
    {"max", c_max},     {"deg", c_deg},     {"rad", c_rad},
    {NULL, NULL}};

int luaopen_libcmath(lua_State *L) {
  luaL_newlib(L, module);
  return 1;
}
