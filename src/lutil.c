#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "lauxlib.h"
#include "lua.h"
#include "lutil.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#define NOGDI 1
#include <ctype.h>
#include <windows.h>
#else
#include <time.h>
#endif

int push_error_string(lua_State *L, const char *info)
{
#ifdef _WIN32
	if (GetLastError() != NO_ERROR) {
		push_windows_error_string(L, GetLastError(), info);
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
	if (GetLastError() != NO_ERROR) {
		push_error_string(L, info);
		lua_pushinteger(L, GetLastError());
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

#ifdef _WIN32
int push_windows_error_string(lua_State *L, DWORD error, const char *info)
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

/* Push nil, followed by the Windows error message corresponding to
 * the error number, or a string giving the error value in decimal if
 * no error message is found.  If nresults is -2, always push nil and
 * the error message and return 2 even if error is NO_ERROR.  If
 * nresults is -1 and error is NO_ERROR, then push true and return 1.
 * Otherwise, if error is NO_ERROR, return nresults.
 */
int windows_pusherror(lua_State *L, DWORD error, int nresults)
{
	if (error != NO_ERROR || nresults == -2) {
		lua_pushnil(L);
		push_windows_error_string(L, error, NULL);
		nresults = 2;
	} else if (nresults < 0) {
		lua_pushboolean(L, 1);
		nresults = 1;
	}
	return nresults;
}
#endif

int get_sleep_divider_from_stack(lua_State *L, int pos, int def)
{
	switch (lua_type(L, 2)) {
	case LUA_TNUMBER:
		int divider = (int)luaL_checknumber(L, 2);
		if (divider <= 0) {
			return luaL_error(L, "Invalid divider specified");
		}
		return divider;
	case LUA_TSTRING: {
		const char *unit = lua_tostring(L, 2);
		if (strcmp(unit, "s") == 0) {
			return 1;
		} else if (strcmp(unit, "ms") == 0) {
			return 1000;
		} else {
			return luaL_error(L, "Invalid unit specified");
		}
		break;
	}
	case LUA_TNIL:
		return def;
	default:
		return luaL_error(L, "Invalid second parameter type");
	}
}

void sleep_for_fraction(int seconds, int divider)
{
	// Calculate the sleep time in milliseconds
	int sleep_time_in_milliseconds = (seconds * 1000) / divider;

#ifdef _WIN32
	// Windows Sleep function takes time in milliseconds
	Sleep((DWORD)sleep_time_in_milliseconds); // Sleep in milliseconds
#else
	struct timespec ts;

	// Convert milliseconds to seconds and nanoseconds for nanosleep()
	ts.tv_sec = sleep_time_in_milliseconds / 1000; // Whole seconds
	ts.tv_nsec = (sleep_time_in_milliseconds % 1000) *
		     1000000L; // Remaining milliseconds to nanoseconds

	// Sleep using nanosleep
	nanosleep(&ts, NULL);
#endif
}