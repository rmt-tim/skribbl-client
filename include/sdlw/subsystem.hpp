#ifndef SDLW_SUBSYSTEM_INCLUDED
#define SDLW_SUBSYSTEM_INCLUDED

#include <SDL.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

class subsystem {
    u32 _flags;

public:
    // clang-format off
    enum flags : u32 {
        timer           = SDL_INIT_TIMER,
        audio           = SDL_INIT_AUDIO,
        video           = SDL_INIT_VIDEO,
        joystick        = SDL_INIT_JOYSTICK,
        haptic          = SDL_INIT_HAPTIC,
        game_controller = SDL_INIT_GAMECONTROLLER,
        events          = SDL_INIT_EVENTS,
        no_parachute    = SDL_INIT_NOPARACHUTE,
        everything      = SDL_INIT_EVERYTHING
    };
    // clang-format on

    explicit subsystem(u32 flags)
        : _flags(flags)
    {
        if (SDL_InitSubSystem(_flags) < 0) {
            throw error{};
        }
    }

    ~subsystem() noexcept
    {
        SDL_QuitSubSystem(_flags);
    }

    static auto was_init(subsystem::flags flags) noexcept -> subsystem::flags
    {
        return static_cast<subsystem::flags>(SDL_WasInit(static_cast<u32>(flags)));
    }
};

inline void set_main_ready() noexcept
{
    SDL_SetMainReady();
}

} // namespace sdl

#endif // SDLW_SUBSYSTEM_INCLUDED
