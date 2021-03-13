#ifndef SDLW_POWER_INCLUDED
#define SDLW_POWER_INCLUDED

#include <SDL_power.h>

namespace sdl {

// clang-format off

enum class battery_state {
    unknown    = SDL_POWERSTATE_UNKNOWN,
    on_battery = SDL_POWERSTATE_ON_BATTERY,
    no_battery = SDL_POWERSTATE_NO_BATTERY,
    charging   = SDL_POWERSTATE_CHARGING,
    charged    = SDL_POWERSTATE_CHARGED
};

// clang-format on

struct battery_info {
    battery_state state;
    int seconds_left;
    int percentage_left;
};

inline battery_info get_battery_info() noexcept
{
    battery_info info;
    SDL_PowerState power_state = SDL_GetPowerInfo(&info.seconds_left, &info.percentage_left);
    info.state = static_cast<battery_state>(power_state);
    return info;
}

} // namespace sdl

#endif // SDLW_POWER_INCLUDED
