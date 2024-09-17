#ifndef ELI_EXTRA_UTILS_SLEEP_H__
#define ELI_EXTRA_UTILS_SLEEP_H__

#include "lua.h"
#include "lauxlib.h"

int get_sleep_divider_from_state(lua_State *L, int pos, int def);
int sleep_duration_to_ms(int duration, int divider);
void sleep_ms(int ms);
long long get_time_in_ms();

#endif // ELI_EXTRA_UTILS_SLEEP_H__
