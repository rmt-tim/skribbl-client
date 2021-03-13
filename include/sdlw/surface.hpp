#ifndef SDLW_SURFACE_INCLUDED
#define SDLW_SURFACE_INCLUDED

#include <SDL_surface.h>

#include <sdlw/blend_mode.hpp>
#include <sdlw/rwops.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

struct surface_ref;

class surface {
protected:
    SDL_Surface* _handle = nullptr;

public:
    ~surface()
    {
        if (_handle) SDL_FreeSurface(_handle);
    }

    surface() = default;

    surface(surface&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    surface& operator=(surface&& other) noexcept
    {
        if (_handle) SDL_FreeSurface(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit surface(SDL_Surface* handle) noexcept
        : _handle{handle}
    {}

    SDL_Surface* handle() const noexcept
    {
        return _handle;
    }

    surface(surface_ref) = delete;

    surface(int width, int height, int depth, u32 rmask, u32 gmask, u32 bmask, u32 amask)
        : _handle{SDL_CreateRGBSurface(0, width, height, depth, rmask, gmask, bmask, amask)}
    {
        if (!_handle) throw error{};
    }

    surface(void* pixels, int width, int height, int depth, int pitch, u32 rmask, u32 gmask, u32 bmask, u32 amask)
        : _handle{SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, rmask, gmask, bmask, amask)}
    {
        if (!_handle) throw error{};
    }

    surface(int width, int height, int depth, pixel_format_type format)
        : _handle{SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, static_cast<u32>(format))}
    {
        if (!_handle) throw error{};
    }

    surface(void* pixels, int width, int height, int depth, int pitch, pixel_format_type format)
        : _handle{SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height, depth, pitch, static_cast<u32>(format))}
    {
        if (!_handle) throw error{};
    }

    pixel_format_ref format() const noexcept
    {
        return pixel_format_ref{_handle->format};
    }

    sdl::size size() const noexcept
    {
        return {_handle->w, _handle->h};
    }

    int pitch() const noexcept
    {
        return _handle->pitch;
    }

    void fill(const rect& r, u32 color)
    {
        auto rr = reinterpret_cast<const SDL_Rect*>(&r);
        if (SDL_FillRect(_handle, rr, color) < 0) {
            throw error{};
        }
    }

    void fill(span<const rect, int> rects, u32 color)
    {
        auto sdl_rects = reinterpret_cast<const SDL_Rect*>(rects.data());
        auto sdl_rects_size = static_cast<int>(rects.size());
        if (SDL_FillRects(_handle, sdl_rects, sdl_rects_size, color) < 0) {
            throw error{};
        }
    }

    surface convert(pixel_format_ref fmt) const;

    rect clip() const noexcept
    {
        SDL_Rect r;
        SDL_GetClipRect(_handle, &r);
        return rect{r.x, r.y, r.w, r.h};
    }

    u32 color_key() const
    {
        u32 key;
        if (SDL_GetColorKey(_handle, &key) < 0) {
            throw error{};
        }
        return key;
    }

    u8 alpha_mod() const
    {
        u8 alpha;
        if (SDL_GetSurfaceAlphaMod(_handle, &alpha) < 0) {
            throw error{};
        }
        return alpha;
    }

    sdl::blend_mode blend_mode() const
    {
        SDL_BlendMode bm;
        if (SDL_GetSurfaceBlendMode(_handle, &bm) < 0) {
            throw error{};
        }
        return static_cast<sdl::blend_mode>(bm);
    }

    std::array<u8, 3> color_mod() const
    {
        u8 r, g, b;
        if (SDL_GetSurfaceColorMod(_handle, &r, &g, &b) < 0) {
            throw error{};
        }
        return {r, g, b};
    }

    void lock()
    {
        if (SDL_LockSurface(_handle) < 0) {
            throw error{};
        }
    }

    bool must_lock() const noexcept
    {
        return static_cast<bool>(SDL_MUSTLOCK(_handle));
    }

    bool set_clip(const rect& r)
    {
        auto rr = reinterpret_cast<const SDL_Rect*>(&r);
        return static_cast<bool>(SDL_SetClipRect(_handle, rr));
    }

    void set_color_key(bool flag, u32 key)
    {
        if (SDL_SetColorKey(_handle, static_cast<int>(flag), key) < 0) {
            throw error{};
        }
    }

    void set_alpha_mod(u8 alpha)
    {
        if (SDL_SetSurfaceAlphaMod(_handle, alpha) < 0) {
            throw error{};
        }
    }

    void set_blend_mode(sdl::blend_mode bm)
    {
        if (SDL_SetSurfaceBlendMode(_handle, static_cast<SDL_BlendMode>(bm)) < 0) {
            throw error{};
        }
    }

    void set_color_mod(u8 r, u8 g, u8 b)
    {
        if (SDL_SetSurfaceColorMod(_handle, r, g, b) < 0) {
            throw error{};
        }
    }

    void set_palette(palette_ref p)
    {
        if (SDL_SetSurfacePalette(_handle, p.handle()) < 0) {
            throw error{};
        }
    }

    void set_rle(bool flag)
    {
        if (SDL_SetSurfaceRLE(_handle, static_cast<int>(flag)) < 0) {
            throw error{};
        }
    }

    void unlock() noexcept
    {
        SDL_UnlockSurface(_handle);
    }
};

inline surface surface::convert(pixel_format_ref fmt) const
{
    SDL_Surface* handle = SDL_ConvertSurface(_handle, fmt.handle(), 0);
    if (!handle) throw error{};
    return surface{handle};
}

struct surface_ref : surface {
    ~surface_ref()
    {
        _handle = nullptr;
    }

    surface_ref(const surface_ref& other) noexcept
        : surface{other._handle}
    {}

    surface_ref(const surface& other) noexcept
        : surface{other.handle()}
    {}

    explicit surface_ref(SDL_Surface* handle) noexcept
        : surface{handle}
    {}

    SDL_Surface* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

inline void blit_scaled(const surface& src, surface& dst)
{
    if (SDL_BlitScaled(src.handle(), nullptr, dst.handle(), nullptr) < 0) {
        throw error{};
    }
}

inline void blit_scaled(const surface& src, const rect& srcrect, surface& dst)
{
    auto r = reinterpret_cast<const SDL_Rect*>(&srcrect);
    if (SDL_BlitScaled(src.handle(), r, dst.handle(), nullptr) < 0) {
        throw error{};
    }
}

inline void blit_scaled(const surface& src, surface& dst, rect& dstrect)
{
    auto r = reinterpret_cast<SDL_Rect*>(&dstrect);
    if (SDL_BlitScaled(src.handle(), nullptr, dst.handle(), r) < 0) {
        throw error{};
    }
}

inline void blit_scaled(const surface& src, const rect& srcrect, surface& dst, rect& dstrect)
{
    auto srcr = reinterpret_cast<const SDL_Rect*>(&srcrect);
    auto dstr = reinterpret_cast<SDL_Rect*>(&dstrect);
    if (SDL_BlitScaled(src.handle(), srcr, dst.handle(), dstr) < 0) {
        throw error{};
    }
}

inline void blit(const surface& src, surface& dst, rect& dstrect)
{
    auto r = reinterpret_cast<SDL_Rect*>(&dstrect);
    if (SDL_BlitSurface(src.handle(), nullptr, dst.handle(), r) < 0) {
        throw error{};
    }
}

inline void blit(const surface& src, const rect& srcrect, surface& dst, rect& dstrect)
{
    auto srcr = reinterpret_cast<const SDL_Rect*>(&srcrect);
    auto dstr = reinterpret_cast<SDL_Rect*>(&dstrect);
    if (SDL_BlitSurface(src.handle(), srcr, dst.handle(), dstr) < 0) {
        throw error{};
    }
}

inline void convert_pixels(
    int width,
    int height,
    pixel_format_type src_format,
    const void* src,
    int src_pitch,
    pixel_format_type dst_format,
    void* dst,
    int dst_pitch)
{
    const auto srcfmt = static_cast<u32>(src_format);
    const auto dstfmt = static_cast<u32>(dst_format);
    if (SDL_ConvertPixels(width, height, srcfmt, src, src_pitch, dstfmt, dst, dst_pitch) < 0) {
        throw error{};
    }
}

inline surface load_bmp(const char* file)
{
    SDL_Surface* handle = SDL_LoadBMP(file);
    if (!handle) throw error{};
    return surface{handle};
}

inline surface load_bmp(stream& s)
{
    SDL_Surface* handle = SDL_LoadBMP_RW(s.get_pointer(), 0);
    if (!handle) throw error{};
    return surface{handle};
}

// TODO: These SDL functions take src by pointer to non-const. Was this
// intended?

inline void lower_blit(const surface& src, surface& dst, rect& dstrect)
{
    auto r = reinterpret_cast<SDL_Rect*>(&dstrect);
    if (SDL_LowerBlit(src.handle(), nullptr, dst.handle(), r) < 0) {
        throw error{};
    }
}

inline void lower_blit(const surface& src, rect& srcrect, surface& dst, rect& dstrect)
{
    auto srcr = reinterpret_cast<SDL_Rect*>(&srcrect);
    auto dstr = reinterpret_cast<SDL_Rect*>(&dstrect);
    if (SDL_LowerBlit(src.handle(), srcr, dst.handle(), dstr) < 0) {
        throw error{};
    }
}

inline void lower_blit_scaled(const surface& src, rect& srcrect, surface& dst, rect& dstrect)
{
    auto srcr = reinterpret_cast<SDL_Rect*>(&srcrect);
    auto dstr = reinterpret_cast<SDL_Rect*>(&dstrect);
    if (SDL_LowerBlitScaled(src.handle(), srcr, dst.handle(), dstr) < 0) {
        throw error{};
    }
}

inline void save_bmp(const surface& s, const char* file)
{
    if (SDL_SaveBMP(s.handle(), file) < 0) {
        throw error{};
    }
}

inline void save_bmp(const surface& surf, stream& s)
{
    if (SDL_SaveBMP_RW(surf.handle(), s.get_pointer(), 0) < 0) {
        throw error{};
    }
}

} // namespace sdl

#endif // SDLW_SURFACE_INCLUDED
