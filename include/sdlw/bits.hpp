#ifndef SDLW_BITS_INCLUDED
#define SDLW_BITS_INCLUDED

#include <SDL_bits.h>

#include <sdlw/types.hpp>

namespace sdl {

inline int most_significant_bit_index(u32 x) noexcept
{
    return SDL_MostSignificantBitIndex32(x);
}

} // namespace sdl

#endif // SDLW_BITS_INCLUDED
