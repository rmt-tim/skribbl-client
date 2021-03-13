#ifndef SDLW_RENDER_INCLUDED
#define SDLW_RENDER_INCLUDED

#include <SDL_render.h>

#include <sdlw/blend_mode.hpp>
#include <sdlw/pixels.hpp>
#include <sdlw/rect.hpp>
#include <sdlw/surface.hpp>
#include <sdlw/video.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

// clang-format off

enum renderer_flip : u32 {
    flip_none       = SDL_FLIP_NONE,
    flip_horizontal = SDL_FLIP_HORIZONTAL,
    flip_vertical   = SDL_FLIP_VERTICAL
};

// clang-format on

class renderer_info;
class texture;
struct texture_ref;
struct renderer_ref;

class renderer {
protected:
    SDL_Renderer* _handle = nullptr;

public:
    ~renderer()
    {
        if (_handle) SDL_DestroyRenderer(_handle);
    }

    renderer() = default;

    renderer(renderer&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    renderer& operator=(renderer&& other) noexcept
    {
        if (_handle) SDL_DestroyRenderer(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit renderer(SDL_Renderer* handle) noexcept
        : _handle{handle}
    {}

    SDL_Renderer* handle() const noexcept
    {
        return _handle;
    }

    renderer(renderer_ref) = delete;

    enum flags {
        software       = SDL_RENDERER_SOFTWARE,
        accelerated    = SDL_RENDERER_ACCELERATED,
        present_vsync  = SDL_RENDERER_PRESENTVSYNC,
        target_texture = SDL_RENDERER_TARGETTEXTURE
    };

    renderer(window& win, u32 flags, int rendering_driver_index = -1)
        : _handle{SDL_CreateRenderer(win.handle(), rendering_driver_index, static_cast<u32>(flags))}
    {
        if (!_handle) throw error{};
    }

    explicit renderer(surface& s)
        : renderer{SDL_CreateSoftwareRenderer(s.handle())}
    {
        if (!_handle) throw error{};
    }

    blend_mode draw_blend_mode() const
    {
        SDL_BlendMode mode;
        if (SDL_GetRenderDrawBlendMode(_handle, &mode) != 0) {
            throw error{};
        }
        return static_cast<blend_mode>(mode);
    }

    void set_draw_blend_mode(blend_mode mode)
    {
        SDL_BlendMode sdl_blend_mode = static_cast<SDL_BlendMode>(mode);
        if (SDL_SetRenderDrawBlendMode(_handle, sdl_blend_mode) < 0) {
            throw error{};
        }
    }

    color draw_color() const
    {
        color c;
        if (SDL_GetRenderDrawColor(_handle, &c.r, &c.g, &c.b, &c.a) != 0) {
            throw error{};
        }
        return c;
    }

    void set_draw_color(color c)
    {
        if (SDL_SetRenderDrawColor(_handle, c.r, c.g, c.b, c.a) < 0) {
            throw error{};
        }
    }

    bool are_targets_supported() const noexcept
    {
        return SDL_RenderTargetSupported(_handle);
    }

    texture_ref target();

    void set_target(texture&);

    size output_size() const
    {
        sdl::size s;
        if (SDL_GetRendererOutputSize(_handle, &s.width, &s.height) != 0) {
            throw error{};
        }
        return s;
    }

    bool is_clip_enabled() const noexcept
    {
        return SDL_RenderIsClipEnabled(_handle);
    }

    rect clip() const noexcept
    {
        SDL_Rect r;
        SDL_RenderGetClipRect(_handle, &r);
        return {r.x, r.y, r.w, r.h};
    }

    void set_clip(const rect& rect)
    {
        auto r = reinterpret_cast<const SDL_Rect*>(&rect);
        if (SDL_RenderSetClipRect(_handle, r) < 0) {
            throw error{};
        }
    }

    rect viewport() const noexcept
    {
        SDL_Rect r;
        SDL_RenderGetViewport(_handle, &r);
        return {r.x, r.y, r.w, r.h};
    }

    void set_viewport(const rect& viewport)
    {
        auto r = reinterpret_cast<const SDL_Rect*>(&viewport);
        if (SDL_RenderSetViewport(_handle, r) < 0) {
            throw error{};
        }
    }

    std::pair<float, float> scale() const noexcept
    {
        float xscale;
        float yscale;
        SDL_RenderGetScale(_handle, &xscale, &yscale);
        return std::make_pair(xscale, xscale);
    }

    void set_scale(float xscale, float yscale)
    {
        if (SDL_RenderSetScale(_handle, xscale, yscale) < 0) {
            throw error{};
        }
    }

    bool integer_scale() const noexcept
    {
        return SDL_RenderGetIntegerScale(_handle);
    }

    void set_integer_scale(bool should_enable)
    {
        SDL_bool b = static_cast<SDL_bool>(should_enable);
        if (SDL_RenderSetIntegerScale(_handle, b) < 0) {
            throw error{};
        }
    }

    size logical_size() const noexcept
    {
        sdl::size s;
        SDL_RenderGetLogicalSize(_handle, &s.width, &s.height);
        return s;
    }

    void set_logical_size(const size& s)
    {
        if (SDL_RenderSetLogicalSize(_handle, s.width, s.height) < 0) {
            throw error{};
        }
    }

    renderer_info info() const;

    void read_pixels(void* pixels, const rect& r, pixel_format_type format, int pitch) const
    {
        auto rr = reinterpret_cast<const SDL_Rect*>(&r);
        if (SDL_RenderReadPixels(_handle, rr, static_cast<u32>(format), pixels, pitch) < 0) {
            throw error{};
        }
    }

    void clear()
    {
        if (SDL_RenderClear(_handle) < 0) {
            throw error{};
        }
    }

    void draw_line(const point& p1, const point& p2)
    {
        if (SDL_RenderDrawLine(_handle, p1.x, p1.y, p2.x, p2.y) < 0) {
            throw error{};
        }
    }

    void draw_line_strip(span<const point> points)
    {
        if (!points.data()) {
            return;
        }
        auto sdl_points = reinterpret_cast<const SDL_Point*>(points.data());
        auto sdl_points_size = static_cast<int>(points.size());
        if (SDL_RenderDrawLines(_handle, sdl_points, sdl_points_size) < 0) {
            throw error{};
        }
    }

    void draw_point(const point& p)
    {
        if (SDL_RenderDrawPoint(_handle, p.x, p.y) < 0) {
            throw error{};
        }
    }

    void draw_points(span<const point> points)
    {
        if (!points.data()) {
            return;
        }
        auto sdl_points = reinterpret_cast<const SDL_Point*>(points.data());
        auto sdl_points_size = static_cast<int>(points.size());
        if (SDL_RenderDrawPoints(_handle, sdl_points, sdl_points_size) < 0) {
            throw error{};
        }
    }

    void draw_rectangle(const rect& r)
    {
        auto rr = reinterpret_cast<const SDL_Rect*>(&r);
        if (SDL_RenderDrawRect(_handle, rr) < 0) {
            throw error{};
        }
    }

    void draw_rectangles(span<const rect> rectangles)
    {
        if (!rectangles.data()) {
            return;
        }
        auto sdl_rectangles = reinterpret_cast<const SDL_Rect*>(rectangles.data());
        auto sdl_rectangles_size = static_cast<int>(rectangles.size());
        if (SDL_RenderDrawRects(_handle, sdl_rectangles, sdl_rectangles_size) == 0) {
            throw error{};
        }
    }

    void fill_rectangle(const rect& r)
    {
        auto rr = reinterpret_cast<const SDL_Rect*>(&r);
        if (SDL_RenderFillRect(_handle, rr) < 0) {
            throw error{};
        }
    }

    void fill_rectangles(span<const rect> rectangles)
    {
        if (!rectangles.data()) return;
        auto sdl_rectangles = reinterpret_cast<const SDL_Rect*>(rectangles.data());
        auto sdl_rectangles_size = static_cast<int>(rectangles.size());
        if (SDL_RenderFillRects(_handle, sdl_rectangles, sdl_rectangles_size) < 0) {
            throw error{};
        }
    }

    void copy(const texture&, const rect* src, const rect* dst);

    void copy(const texture&, const rect* src, const rect* dst, double angle, const point* center, renderer_flip);

    void present() noexcept
    {
        SDL_RenderPresent(_handle);
    }

    friend bool operator==(const renderer& x, const renderer& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const renderer& x, const renderer& y) noexcept
    {
        return !(x == y);
    }
};

struct renderer_ref : renderer {
    ~renderer_ref()
    {
        _handle = nullptr;
    }

    renderer_ref(const renderer_ref& other) noexcept
        : renderer{other._handle}
    {}

    renderer_ref(const renderer& other) noexcept
        : renderer{other.handle()}
    {}

    explicit renderer_ref(SDL_Renderer* handle) noexcept
        : renderer{handle}
    {}

    SDL_Renderer* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

inline renderer_ref window::renderer()
{
    SDL_Renderer* handle = SDL_GetRenderer(_handle);
    if (!handle) throw error{};
    return renderer_ref{handle};
}

class renderer_info {
    SDL_RendererInfo _renderer_info;

public:
    explicit operator SDL_RendererInfo() const noexcept
    {
        return _renderer_info;
    }

    renderer_info() = default;

    explicit renderer_info(const SDL_RendererInfo& info) noexcept
        : _renderer_info{info}
    {}

    auto name() const noexcept -> const char*
    {
        return _renderer_info.name;
    }

    auto flags() const noexcept -> renderer::flags
    {
        return static_cast<renderer::flags>(_renderer_info.flags);
    }

    auto num_texture_formats() const noexcept -> int
    {
        return _renderer_info.num_texture_formats;
    }

    auto texture_format(int index) const noexcept -> pixel_format_type
    {
        const auto tex_format = _renderer_info.texture_formats[index];
        return static_cast<pixel_format_type>(tex_format);
    }

    auto max_texture_size() const noexcept -> size
    {
        const auto width = _renderer_info.max_texture_width;
        const auto height = _renderer_info.max_texture_height;
        return {width, height};
    }
};

inline renderer_info renderer::info() const
{
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(_handle, &info) < 0) {
        throw error{};
    }
    return renderer_info{info};
}

// clang-format off

enum class texture_access : int {
    static_   = SDL_TEXTUREACCESS_STATIC,
    streaming = SDL_TEXTUREACCESS_STREAMING,
    target    = SDL_TEXTUREACCESS_TARGET
};

// clang-format on

class texture {
protected:
    SDL_Texture* _handle = nullptr;

public:
    ~texture()
    {
        if (_handle) SDL_DestroyTexture(_handle);
    }

    texture() = default;

    texture(texture&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    texture& operator=(texture&& other) noexcept
    {
        if (_handle) SDL_DestroyTexture(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit texture(SDL_Texture* handle) noexcept
        : _handle{handle}
    {}

    SDL_Texture* handle() const noexcept
    {
        return _handle;
    }

    texture(texture_ref) = delete;

    texture(const renderer& r, pixel_format_type format, texture_access access, const sdl::size& sz)
        : texture{SDL_CreateTexture(r.handle(), static_cast<u32>(format), static_cast<int>(access), sz.width, sz.height)}
    {
        if (!_handle) throw error{};
    }

    texture(const renderer& rend, const surface& surf)
        : texture{SDL_CreateTextureFromSurface(rend.handle(), surf.handle())}
    {
        if (!_handle) throw error{};
    }

    texture_access access() const
    {
        int access;
        if (SDL_QueryTexture(_handle, nullptr, &access, nullptr, nullptr) != 0) {
            throw error{};
        }
        return static_cast<texture_access>(access);
    }

    u8 alpha_mod() const
    {
        u8 alpha;
        if (SDL_GetTextureAlphaMod(_handle, &alpha) != 0) {
            throw error{};
        }
        return alpha;
    }

    sdl::blend_mode blend_mode() const
    {
        SDL_BlendMode mode;
        if (SDL_GetTextureBlendMode(_handle, &mode) != 0) {
            throw error{};
        }
        return static_cast<sdl::blend_mode>(mode);
    }

    std::array<u8, 3> color_mod() const
    {
        u8 r, g, b;
        if (SDL_GetTextureColorMod(_handle, &r, &g, &b) != 0) {
            throw error{};
        }
        return std::array<u8, 3>{r, g, b};
    }

    pixel_format_type format() const
    {
        u32 format;
        if (SDL_QueryTexture(_handle, &format, nullptr, nullptr, nullptr) != 0) {
            throw error{};
        }
        return static_cast<pixel_format_type>(format);
    }

    sdl::size size() const
    {
        int width, height;
        if (SDL_QueryTexture(_handle, nullptr, nullptr, &width, &height) != 0) {
            throw error{};
        }
        return sdl::size{width, height};
    }

    void set_alpha_mod(u8 alpha)
    {
        if (SDL_SetTextureAlphaMod(_handle, alpha) < 0) {
            throw error{};
        }
    }

    void set_blend_mode(sdl::blend_mode mode)
    {
        if (SDL_SetTextureBlendMode(_handle, static_cast<SDL_BlendMode>(mode)) < 0) {
            throw error{};
        }
    }

    void set_color_mod(u8 red, u8 green, u8 blue)
    {
        if (SDL_SetTextureColorMod(_handle, red, green, blue) < 0) {
            throw error{};
        }
    }

    std::tuple<void*, int> lock(const rect& area)
    {
        auto r = reinterpret_cast<const SDL_Rect*>(&area);
        void* pixels;
        int pitch;
        if (SDL_LockTexture(_handle, r, &pixels, &pitch) != 0) {
            throw error{};
        }
        return std::make_tuple(pixels, pitch);
    }

    void unlock() noexcept
    {
        SDL_UnlockTexture(_handle);
    }

    void update(const rect& r, const void* pixels, int pitch)
    {
        auto rr = reinterpret_cast<const SDL_Rect*>(&r);
        if (SDL_UpdateTexture(_handle, rr, pixels, pitch) < 0) {
            throw error{};
        }
    }

    void update_yuv(const rect& r, const u8* yplane, int ypitch, const u8* uplane, int upitch, const u8* vplane, int vpitch)
    {
        auto rr = reinterpret_cast<const SDL_Rect*>(&r);
        if (SDL_UpdateYUVTexture(_handle, rr, yplane, ypitch, uplane, upitch, vplane, vpitch) < 0) {
            throw error{};
        }
    }
};

inline void renderer::set_target(texture& t)
{
    if (SDL_SetRenderTarget(_handle, t.handle()) < 0) {
        throw error{};
    }
}

inline void renderer::copy(const texture& tex, const rect* source, const rect* destination)
{
    auto src = reinterpret_cast<const SDL_Rect*>(source);
    auto dst = reinterpret_cast<const SDL_Rect*>(destination);
    if (SDL_RenderCopy(_handle, tex.handle(), src, dst) < 0) {
        throw error{};
    }
}

inline void renderer::copy(const texture& t, const rect* source, const rect* destination, double angle, const point* center, renderer_flip f)
{
    auto src = reinterpret_cast<const SDL_Rect*>(source);
    auto dst = reinterpret_cast<const SDL_Rect*>(destination);
    auto c = reinterpret_cast<const SDL_Point*>(center);
    if (SDL_RenderCopyEx(_handle, t.handle(), src, dst, angle, c, static_cast<SDL_RendererFlip>(f)) < 0) {
        throw error{};
    }
}

struct texture_ref : texture {
    ~texture_ref()
    {
        _handle = nullptr;
    }

    texture_ref(const texture_ref& other) noexcept
        : texture{other._handle}
    {}

    texture_ref(const texture& other) noexcept
        : texture{other.handle()}
    {}

    explicit texture_ref(SDL_Texture* handle) noexcept
        : texture{handle}
    {}

    SDL_Texture* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

inline texture_ref renderer::target()
{
    SDL_Texture* handle = SDL_GetRenderTarget(_handle);
    if (!handle) throw error{};
    return texture_ref{handle};
}

struct render_driver {
    int index;

    renderer_info info() const
    {
        SDL_RendererInfo info;
        if (SDL_GetRenderDriverInfo(index, &info) != 0) {
            throw error{};
        }
        return renderer_info{info};
    }

    static int count()
    {
        int result = SDL_GetNumRenderDrivers();
        if (result < 0) throw error{};
        return result;
    }
};

inline std::pair<window, renderer> make_window_and_renderer(const size& window_size, enum window::flags flags)
{
    SDL_Window* pwindow;
    SDL_Renderer* prenderer;
    const auto flags_ = static_cast<u32>(flags);
    if (SDL_CreateWindowAndRenderer(window_size.width, window_size.height, flags_, &pwindow, &prenderer) != 0) {
        throw error{};
    }
    return std::make_pair(window{pwindow}, renderer{prenderer});
}

namespace gl {

inline std::pair<float, float> bind_texture(texture& t)
{
    float texw, texh;
    if (SDL_GL_BindTexture(t.handle(), &texw, &texh) < 0) {
        throw error{};
    }
    return std::make_pair(texw, texh);
}

inline void unbind_texture(texture& t)
{
    if (SDL_GL_UnbindTexture(t.handle()) < 0) {
        throw error{};
    }
}

} // namespace gl

} // namespace sdl

#endif // SDLW_RENDER_INCLUDED
