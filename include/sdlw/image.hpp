#ifndef SDLW_IMAGE_INCLUDED
#define SDLW_IMAGE_INCLUDED

#include <SDL_image.h>

#include <sdlw/render.hpp>
#include <sdlw/surface.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {
namespace img {

struct subsystem {
    // clang-format off
    enum flags : int {
        jpg  = IMG_INIT_JPG,
        png  = IMG_INIT_PNG,
        tif  = IMG_INIT_TIF,
        webp = IMG_INIT_WEBP
    };
    // clang-format on

    explicit subsystem(int flags)
    {
        if (IMG_Init(flags) < 0) {
            throw error{};
        }
    }

    ~subsystem()
    {
        IMG_Quit();
    }
};

inline surface load_as_surface(const char* filename)
{
    if (const auto psurface = IMG_Load(filename)) {
        return surface{psurface};
    } else {
        throw error{};
    }
}

inline texture load_as_texture(const renderer& rend, const char* filename)
{
    const auto ptexture = IMG_LoadTexture(rend.handle(), filename);
    if (ptexture) {
        return texture{ptexture};
    } else {
        throw error{};
    }
}

inline void save_as_png(const surface& surf, const char* filename)
{
    if (IMG_SavePNG(surf.handle(), filename) < 0) {
        throw error{};
    }
}

inline void save_as_jpg(const surface& surf, const char* filename, int quality)
{
    if (IMG_SaveJPG(surf.handle(), filename, quality) < 0) {
        throw error{};
    }
}

} // namespace img
} // namespace sdl

#endif // SDLW_IMAGE_INCLUDED
