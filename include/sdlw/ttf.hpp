#ifndef SDLW_TTF_INCLUDED
#define SDLW_TTF_INCLUDED

#include <SDL_ttf.h>

#include <sdlw/error.hpp>
#include <sdlw/pixels.hpp>
#include <sdlw/rect.hpp>
#include <sdlw/surface.hpp>
#include <sdlw/types.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {
namespace ttf {

struct subsystem {
    subsystem()
    {
        if (TTF_Init() < 0) {
            throw error{};
        }
    }

    ~subsystem() noexcept
    {
        TTF_Quit();
    }
};

// clang-format off

enum style : int {
    style_normal        = TTF_STYLE_NORMAL,
    style_bold          = TTF_STYLE_BOLD,
    style_italic        = TTF_STYLE_ITALIC,
    style_underline     = TTF_STYLE_UNDERLINE,
    style_strikethrough = TTF_STYLE_STRIKETHROUGH
};

enum class hinting : int {
    normal = TTF_HINTING_NORMAL,
    light  = TTF_HINTING_LIGHT,
    mono   = TTF_HINTING_MONO,
    none   = TTF_HINTING_NONE
};

// clang-format on

struct glyph_metrics {
    int minx;
    int maxx;
    int miny;
    int maxy;
    int advance;
};

class font {
    TTF_Font* _handle = nullptr;

public:
    ~font()
    {
        if (_handle) TTF_CloseFont(_handle);
    }

    font() = default;

    font(font&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    font& operator=(font&& other) noexcept
    {
        if (_handle) TTF_CloseFont(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit font(TTF_Font* handle) noexcept
        : _handle{handle}
    {}

    TTF_Font* handle() const noexcept
    {
        return _handle;
    }

    font(const char* filename, int ptsize)
        : _handle{TTF_OpenFont(filename, ptsize)}
    {
        if (!_handle) throw error{};
    }

    font(const char* filename, int ptsize, long index)
        : _handle{TTF_OpenFontIndex(filename, ptsize, index)}
    {
        if (!_handle) throw error{};
    }

    int style() const noexcept
    {
        return TTF_GetFontStyle(_handle);
    }

    void set_style(int style) noexcept
    {
        TTF_SetFontStyle(_handle, static_cast<int>(style));
    }

    int outline() const noexcept
    {
        return TTF_GetFontOutline(_handle);
    }

    void set_outline(int outline) noexcept
    {
        TTF_SetFontOutline(_handle, outline);
    }

    sdl::ttf::hinting hinting() const noexcept
    {
        return static_cast<sdl::ttf::hinting>(TTF_GetFontHinting(_handle));
    }

    void set_hinting(sdl::ttf::hinting hinting) noexcept
    {
        TTF_SetFontHinting(_handle, static_cast<int>(hinting));
    }

    bool is_kerning_allowed() const noexcept
    {
        return static_cast<bool>(TTF_GetFontKerning(_handle));
    }

    void set_kerning(bool allowed) noexcept
    {
        TTF_SetFontKerning(_handle, static_cast<int>(allowed));
    }

    int height() const noexcept
    {
        return TTF_FontHeight(_handle);
    }

    int ascent() const noexcept
    {
        return TTF_FontAscent(_handle);
    }

    int descent() const noexcept
    {
        return TTF_FontDescent(_handle);
    }

    int line_skip() const noexcept
    {
        return TTF_FontLineSkip(_handle);
    }

    int num_faces() const noexcept
    {
        return TTF_FontFaces(_handle);
    }

    bool is_face_fixed_width() const noexcept
    {
        return static_cast<bool>(TTF_FontFaceIsFixedWidth(_handle));
    }

    const char* face_family_name() const noexcept
    {
        return TTF_FontFaceFamilyName(_handle);
    }

    const char* face_style_name() const noexcept
    {
        return TTF_FontFaceStyleName(_handle);
    }

    bool is_glyph_provided(u16 ch) const noexcept
    {
        return static_cast<bool>(TTF_GlyphIsProvided(_handle, ch));
    }

    sdl::ttf::glyph_metrics glyph_metrics(u16 ch) const
    {
        sdl::ttf::glyph_metrics metrics;
        const auto minx = &metrics.minx;
        const auto maxx = &metrics.maxx;
        const auto miny = &metrics.miny;
        const auto maxy = &metrics.maxy;
        const auto adv = &metrics.advance;
        if (TTF_GlyphMetrics(_handle, ch, minx, maxx, miny, maxy, adv) != 0) {
            throw error{};
        }
        return metrics;
    }
};

inline size text_render_size(const font& f, const char* text)
{
    sdl::size size;
    if (TTF_SizeText(f.handle(), text, &size.width, &size.height) != 0) {
        throw error{};
    }
    return size;
}

inline surface solid_text_render(const font& f, const char* text, color fg_color)
{
    SDL_Surface* handle = TTF_RenderText_Solid(f.handle(), text, fg_color);
    if (!handle) throw error{};
    return surface{handle};
}

inline surface solid_utf8_render(const font& f, const char* text, color fg_color)
{
    SDL_Surface* handle = TTF_RenderUTF8_Solid(f.handle(), text, fg_color);
    if (!handle) throw error{};
    return surface{handle};
}

inline surface shaded_text_render(const font& f, const char* text, color fg, color bg)
{
    SDL_Surface* handle = TTF_RenderText_Shaded(f.handle(), text, fg, bg);
    if (!handle) throw error{};
    return surface{handle};
}

inline surface shaded_utf8_render(const font& f, const char* text, color fg, color bg)
{
    SDL_Surface* handle = TTF_RenderUTF8_Shaded(f.handle(), text, fg, bg);
    if (!handle) throw error{};
    return surface{handle};
}

inline surface blended_text_render(const font& f, const char* text, color fg_color)
{
    SDL_Surface* handle = TTF_RenderText_Blended(f.handle(), text, fg_color);
    if (!handle) throw error{};
    return surface{handle};
}

inline surface blended_utf8_render(const font& f, const char* text, color fg_color)
{
    SDL_Surface* handle = TTF_RenderUTF8_Blended(f.handle(), text, fg_color);
    if (!handle) throw error{};
    return surface{handle};
}

inline surface blended_wrapped_text_render(const font& f, const char* txt, color fg, u32 wrap_length)
{
    SDL_Surface* handle = TTF_RenderText_Blended_Wrapped(f.handle(), txt, fg, wrap_length);
    if (!handle) throw error{};
    return surface{handle};
}

inline surface blended_wrapped_utf8_render(const font& f, const char* txt, color fg, u32 wrap_length)
{
    SDL_Surface* handle = TTF_RenderUTF8_Blended_Wrapped(f.handle(), txt, fg, wrap_length);
    if (!handle) throw error{};
    return surface{handle};
}

} // namespace ttf
} // namespace sdl

#endif // SDLW_TTF_INCLUDED
