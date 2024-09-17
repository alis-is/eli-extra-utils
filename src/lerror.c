#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "lerror.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <ctype.h>
#include <windows.h>

static int push_windows_error_string(lua_State *L, DWORD error,
				     const char *info)
{
	if (error == NO_ERROR) {
		lua_pushnil(L);
		return 1;
	}
	char buffer[1024];
	size_t len = sprintf(buffer, "%lu (0x%lX): ", error, error);
	size_t res = FormatMessage(
		FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, 0,
		error, 0, buffer + len, sizeof buffer - len, 0);
	if (res) {
		len += res;
		while (len > 0 && isspace(buffer[len - 1])) {
			len--;
		}
	} else {
		len += sprintf(buffer + len, "<error string not available>");
	}
	if (info != NULL) {
		lua_pushstring(L, info);
		lua_pushstring(L, ": ");
	}
	lua_pushlstring(L, buffer, len);
	if (info != NULL) {
		lua_concat(L, 3);
	}

	return 1;
}
#endif

int push_error_string(lua_State *L, const char *info)
{
#ifdef _WIN32
	DWORD err = GetLastError();
	if (err != NO_ERROR) {
		push_windows_error_string(L, err, info);
		return 1;
	}
#endif
	if (info == NULL) {
		lua_pushstring(L, strerror(errno));
	} else {
		lua_pushfstring(L, "%s: %s", info, strerror(errno));
	}
	return 1;
}

int push_error(lua_State *L, const char *info)
{
	lua_pushnil(L);
#ifdef _WIN32
	DWORD err = GetLastError();
	if (err != NO_ERROR) {
		push_windows_error_string(L, err, info);
		lua_pushinteger(L, err);
		return 3;
	}
#endif
	push_error_string(L, info);
	lua_pushinteger(L, errno);
	return 3;
}

int push_result(lua_State *L, int res, const char *info)
{
	if (res == -1) {
		return push_error(L, info);
	} else {
		lua_pushboolean(L, 1);
		return 1;
	}
}
