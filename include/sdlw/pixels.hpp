#ifndef SDLW_PIXELS_INCLUDED
#define SDLW_PIXELS_INCLUDED

#include <SDL_pixels.h>

#include <sdlw/types.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

using color = SDL_Color;

// clang-format off

enum class alpha : u8 {
    transparent = SDL_ALPHA_TRANSPARENT,
    opaque      = SDL_ALPHA_OPAQUE
};

enum class pixel_type {
    unknown  = SDL_PIXELTYPE_UNKNOWN,
    index1   = SDL_PIXELTYPE_INDEX1,
    index4   = SDL_PIXELTYPE_INDEX4,
    index8   = SDL_PIXELTYPE_INDEX8,
    packed8  = SDL_PIXELTYPE_PACKED8,
    packed16 = SDL_PIXELTYPE_PACKED16,
    packed32 = SDL_PIXELTYPE_PACKED32,
    arrayu8  = SDL_PIXELTYPE_ARRAYU8,
    arrayu16 = SDL_PIXELTYPE_ARRAYU16,
    arrayu32 = SDL_PIXELTYPE_ARRAYU32,
    arrayf16 = SDL_PIXELTYPE_ARRAYF16,
    arrayf32 = SDL_PIXELTYPE_ARRAYF32
};

enum class pixel_order {
    bitmap_none = SDL_BITMAPORDER_NONE,
    bitmap_4321 = SDL_BITMAPORDER_4321,
    bitmap_1234 = SDL_BITMAPORDER_1234,
    packed_none = SDL_PACKEDORDER_NONE,
    packed_xrgb = SDL_PACKEDORDER_XRGB,
    packed_rgbx = SDL_PACKEDORDER_RGBX,
    packed_argb = SDL_PACKEDORDER_ARGB,
    packed_rgba = SDL_PACKEDORDER_RGBA,
    packed_xbgr = SDL_PACKEDORDER_XBGR,
    packed_bgrx = SDL_PACKEDORDER_BGRX,
    packed_abgr = SDL_PACKEDORDER_ABGR,
    packed_bgra = SDL_PACKEDORDER_BGRA,
    array_none  = SDL_ARRAYORDER_NONE,
    array_rgb   = SDL_ARRAYORDER_RGB,
    array_rgba  = SDL_ARRAYORDER_RGBA,
    array_argb  = SDL_ARRAYORDER_ARGB,
    array_bgr   = SDL_ARRAYORDER_BGR,
    array_bgra  = SDL_ARRAYORDER_BGRA,
    array_abgr  = SDL_ARRAYORDER_ABGR
};

enum class pixel_layout {
    none     = SDL_PACKEDLAYOUT_NONE,
    _332     = SDL_PACKEDLAYOUT_332,
    _4444    = SDL_PACKEDLAYOUT_4444,
    _1555    = SDL_PACKEDLAYOUT_1555,
    _5551    = SDL_PACKEDLAYOUT_5551,
    _565     = SDL_PACKEDLAYOUT_565,
    _8888    = SDL_PACKEDLAYOUT_8888,
    _2101010 = SDL_PACKEDLAYOUT_2101010,
    _1010102 = SDL_PACKEDLAYOUT_1010102
};

enum class pixel_format_type : u32 {
    unknown     = SDL_PIXELFORMAT_UNKNOWN,
    index1lsb   = SDL_PIXELFORMAT_INDEX1LSB,
    index1msb   = SDL_PIXELFORMAT_INDEX1MSB,
    index4lsb   = SDL_PIXELFORMAT_INDEX4LSB,
    index4msb   = SDL_PIXELFORMAT_INDEX4MSB,
    index8      = SDL_PIXELFORMAT_INDEX8,
    rgb332      = SDL_PIXELFORMAT_RGB332,
    rgb444      = SDL_PIXELFORMAT_RGB444,
    rgb555      = SDL_PIXELFORMAT_RGB555,
    bgr555      = SDL_PIXELFORMAT_BGR555,
    argb4444    = SDL_PIXELFORMAT_ARGB4444,
    rgba4444    = SDL_PIXELFORMAT_RGBA4444,
    abgr4444    = SDL_PIXELFORMAT_ABGR4444,
    bgra4444    = SDL_PIXELFORMAT_BGRA4444,
    argb1555    = SDL_PIXELFORMAT_ARGB1555,
    rgba5551    = SDL_PIXELFORMAT_RGBA5551,
    abgr1555    = SDL_PIXELFORMAT_ABGR1555,
    bgra5551    = SDL_PIXELFORMAT_BGRA5551,
    rgb565      = SDL_PIXELFORMAT_RGB565,
    bgr565      = SDL_PIXELFORMAT_BGR565,
    rgb24       = SDL_PIXELFORMAT_RGB24,
    bgr24       = SDL_PIXELFORMAT_BGR24,
    rgb888      = SDL_PIXELFORMAT_RGB888,
    rgbx8888    = SDL_PIXELFORMAT_RGBX8888,
    bgr888      = SDL_PIXELFORMAT_BGR888,
    bgrx8888    = SDL_PIXELFORMAT_BGRX8888,
    argb8888    = SDL_PIXELFORMAT_ARGB8888,
    rgba8888    = SDL_PIXELFORMAT_RGBA8888,
    abgr8888    = SDL_PIXELFORMAT_ABGR8888,
    bgra8888    = SDL_PIXELFORMAT_BGRA8888,
    argb2101010 = SDL_PIXELFORMAT_ARGB2101010,
    rgba32      = SDL_PIXELFORMAT_RGBA32,
    argb32      = SDL_PIXELFORMAT_ARGB32,
    bgra32      = SDL_PIXELFORMAT_BGRA32,
    abgr32      = SDL_PIXELFORMAT_ABGR32,
    yv12        = SDL_PIXELFORMAT_YV12,
    iyuv        = SDL_PIXELFORMAT_IYUV,
    yuy2        = SDL_PIXELFORMAT_YUY2,
    uyvy        = SDL_PIXELFORMAT_UYVY,
    yvyu        = SDL_PIXELFORMAT_YVYU,
    nv12        = SDL_PIXELFORMAT_NV12,
    nv21        = SDL_PIXELFORMAT_NV21
};

// clang-format on

constexpr pixel_type type(pixel_format_type pixel_format) noexcept
{
    u32 sdl_pixel_format = static_cast<u32>(pixel_format);
    return static_cast<pixel_type>(SDL_PIXELTYPE(sdl_pixel_format));
}

constexpr pixel_order order(pixel_format_type pixel_format) noexcept
{
    u32 sdl_pixel_format = static_cast<u32>(pixel_format);
    return static_cast<pixel_order>(SDL_PIXELORDER(sdl_pixel_format));
}

constexpr pixel_layout layout(pixel_format_type pixel_format) noexcept
{
    u32 sdl_pixel_format = static_cast<u32>(pixel_format);
    return static_cast<pixel_layout>(SDL_PIXELLAYOUT(sdl_pixel_format));
}

constexpr int bits_per_pixel(pixel_format_type pixel_format) noexcept
{
    return SDL_BITSPERPIXEL(static_cast<u32>(pixel_format));
}

constexpr int bytes_per_pixel(pixel_format_type pixel_format) noexcept
{
    return SDL_BYTESPERPIXEL(static_cast<u32>(pixel_format));
}

constexpr bool is_indexed(pixel_format_type pixel_format) noexcept
{
    return SDL_ISPIXELFORMAT_INDEXED(static_cast<u32>(pixel_format));
}

constexpr bool is_alpha(pixel_format_type pixel_format) noexcept
{
    return SDL_ISPIXELFORMAT_ALPHA(static_cast<u32>(pixel_format));
}

constexpr bool is_fourcc(pixel_format_type pixel_format) noexcept
{
    return SDL_ISPIXELFORMAT_FOURCC(static_cast<u32>(pixel_format));
}

struct palette_ref;

class palette {
protected:
    SDL_Palette* _handle = nullptr;

public:
    ~palette()
    {
        if (_handle) SDL_FreePalette(_handle);
    }

    palette() = default;

    palette(palette&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    palette& operator=(palette&& other) noexcept
    {
        if (_handle) SDL_FreePalette(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit palette(SDL_Palette* handle) noexcept
        : _handle{handle}
    {}

    SDL_Palette* handle() const noexcept
    {
        return _handle;
    }

    palette(palette_ref) = delete;

    explicit palette(int num_colors)
        : _handle{SDL_AllocPalette(num_colors)}
    {
        if (!_handle) throw error{};
    }

    span<color> colors() const noexcept
    {
        return {_handle->colors, _handle->ncolors};
    }
};

struct palette_ref : palette {
    ~palette_ref()
    {
        _handle = nullptr;
    }

    palette_ref(const palette_ref& other) noexcept
        : palette{other._handle}
    {}

    palette_ref(const palette& other) noexcept
        : palette{other.handle()}
    {}

    explicit palette_ref(SDL_Palette* handle) noexcept
        : palette{handle}
    {}

    SDL_Palette* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

struct pixel_format_ref;

class pixel_format {
protected:
    SDL_PixelFormat* _handle = nullptr;

public:
    ~pixel_format()
    {
        if (_handle) SDL_FreeFormat(_handle);
    }

    pixel_format() = default;

    pixel_format(pixel_format&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    pixel_format& operator=(pixel_format&& other) noexcept
    {
        if (_handle) SDL_FreeFormat(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit pixel_format(SDL_PixelFormat* handle) noexcept
        : _handle{handle}
    {}

    SDL_PixelFormat* handle() const noexcept
    {
        return _handle;
    }

    pixel_format(pixel_format_ref) = delete;

    explicit pixel_format(pixel_format_type fmt)
        : _handle{SDL_AllocFormat(static_cast<u32>(fmt))}
    {
        if (!_handle) throw error{};
    }

    pixel_format_type format() const noexcept
    {
        return static_cast<pixel_format_type>(_handle->format);
    }

    palette_ref palette() const noexcept
    {
        return palette_ref{_handle->palette};
    }

    u8 bits_per_pixel() const noexcept
    {
        return _handle->BitsPerPixel;
    }

    u8 bytes_per_pixel() const noexcept
    {
        return _handle->BytesPerPixel;
    }

    std::array<u32, 4> mask() const noexcept
    {
        return {_handle->Rmask, _handle->Gmask, _handle->Bmask, _handle->Amask};
    }
};

struct pixel_format_ref : pixel_format {
    ~pixel_format_ref()
    {
        _handle = nullptr;
    }

    pixel_format_ref(const pixel_format_ref& other) noexcept
        : pixel_format{other._handle}
    {}

    pixel_format_ref(const pixel_format& other) noexcept
        : pixel_format{other.handle()}
    {}

    explicit pixel_format_ref(SDL_PixelFormat* handle) noexcept
        : pixel_format{handle}
    {}

    SDL_PixelFormat* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

} // namespace sdl

#endif // SDLW_PIXELS_INCLUDED
