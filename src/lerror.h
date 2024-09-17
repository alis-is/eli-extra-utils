#ifndef ELI_EXTRA_UTILS_H__
#define ELI_EXTRA_UTILS_H__

#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "lua.h"
#include "lauxlib.h"

int push_error(lua_State *L, const char *info);
int push_result(lua_State *L, int res, const char *info);
int push_error_string(lua_State *L, const char *info);

#endif