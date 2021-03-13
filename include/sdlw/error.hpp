#ifndef SDLW_ERROR_INCLUDED
#define SDLW_ERROR_INCLUDED

#include <stdexcept>

#include <SDL_error.h>

namespace sdl {

struct error : std::runtime_error {
    error() noexcept
        : std::runtime_error{SDL_GetError()}
    {}
};

inline void clear_error() noexcept
{
    SDL_ClearError();
}

inline const char* get_error() noexcept
{
    return SDL_GetError();
}

template<typename... Args>
void set_error(const char* fmt, Args... args) noexcept
{
    SDL_SetError(fmt, args...);
}

} // namespace sdl

#endif // SDLW_ERROR_INCLUDED
