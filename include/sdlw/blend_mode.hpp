#ifndef SDLW_BLEND_MODE_INCLUDED
#define SDLW_BLEND_MODE_INCLUDED

#include <type_traits>

#include <SDL_blendmode.h>

namespace sdl {

// clang-format off

enum class blend_mode : typename std::underlying_type<SDL_BlendMode>::type {
    none  = SDL_BLENDMODE_NONE,
    blend = SDL_BLENDMODE_BLEND,
    add   = SDL_BLENDMODE_ADD,
    mod   = SDL_BLENDMODE_MOD
};

// clang-format on

} // namespace sdl

#endif // SDLW_BLEND_MODE_INCLUDED
