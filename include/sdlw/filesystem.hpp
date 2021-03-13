#ifndef SDLW_FILESYSTEM_INCLUDED
#define SDLW_FILESYSTEM_INCLUDED

#include <SDL_filesystem.h>

#include <sdlw/error.hpp>

namespace sdl {

inline string base_path()
{
    char* base_path = SDL_GetBasePath();
    if (!base_path) throw error{};
    return string{base_path};
}

inline string get_pref_path(const char* org, const char* app)
{
    char* pref_path = SDL_GetPrefPath(org, app);
    if (!pref_path) throw error{};
    return string{pref_path};
}

} // namespace sdl

#endif // SDLW_FILESYSTEM_INCLUDED
