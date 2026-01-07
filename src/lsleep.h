#ifndef ELI_EXTRA_UTILS_SLEEP_H__
#define ELI_EXTRA_UTILS_SLEEP_H__

#include "lua.h"
#include "lauxlib.h"

double get_ms_divider_from_state(lua_State *L, int pos, double def);
int sleep_ms(double milliseconds);
long long get_time_in_ms();

#endif // ELI_EXTRA_UTILS_SLEEP_H__
