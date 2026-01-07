#include <time.h>
#include <string.h>
#include "lsleep.h"

#if defined(_WIN32) || defined(__APPLE__)
#include "c11threads.h"
#else
#include <threads.h>
#endif

double get_ms_divider_from_state(lua_State *L, int pos, double def)
{
	switch (lua_type(L, pos)) {
	case LUA_TNUMBER: {
		int divider = (int)luaL_checknumber(L, pos);
		if (divider <= 0 ||
		    divider > 1000) { // we dont allow less than 1 us
			return luaL_error(L, "has to be > 0 and <= 1000");
		}
		return divider;
	}
	case LUA_TSTRING: {
		const char *unit = lua_tostring(L, pos);
		if (strcmp(unit, "s") == 0) {
			return 0.001;
		} else if (strcmp(unit, "ms") == 0) {
			return 1.0;
		} else if (strcmp(unit, "us") == 0) {
			return 1000.0;
		} else {
			return luaL_argerror(L, pos,
					     "number, 's' or 'ms' expected");
		}
		break;
	}
	case LUA_TNIL:
	case LUA_TNONE:
		return def;
	default:
		return luaL_argerror(L, pos, "number, 's' or 'ms' expected");
	}
}

int sleep_seconds(double seconds)
{
	if (seconds < 0)
		seconds = 0;

	time_t sec = (time_t)(seconds);

	double remainder_s = seconds - ((double)sec);
	long nsec = (long)(remainder_s * 1000000000.0);

	struct timespec ts = { .tv_sec = sec, .tv_nsec = nsec };

	return thrd_sleep(&ts, NULL);
}

int sleep_ms(double milliseconds)
{
	return sleep_seconds(milliseconds / 1000.0);
}

long long get_time_in_ms()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);

	// Convert seconds to milliseconds and add the nanoseconds converted to milliseconds
	long long time_in_ms = (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);

	return time_in_ms;
}