#ifndef SDLW_PLATFORM_INCLUDED
#define SDLW_PLATFORM_INCLUDED

#include <SDL_platform.h>

namespace sdl {

inline const char* platform() noexcept
{
    return SDL_GetPlatform();
}

} // namespace sdl

#endif // SDLW_PLATFORM_INCLUDED
