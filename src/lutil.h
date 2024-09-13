#ifndef ELI_EXTRA_UTILS_H__
#define ELI_EXTRA_UTILS_H__

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "lua.h"

int push_error(lua_State *L, const char *info);
int push_result(lua_State *L, int res, const char *info);
int push_error_string(lua_State *L, const char *info);

#ifdef _WIN32
#include <ctype.h>
#include <windows.h>
int push_windows_error_string(lua_State *L, DWORD error, const char *info);
int windows_pusherror(lua_State *L, DWORD error, int nresults);
#define windows_pushlasterror(L) windows_pusherror(L, GetLastError(), -2)
#endif

void sleep_for_fraction(int seconds, int divider);

#endif