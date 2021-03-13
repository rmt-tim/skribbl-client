#ifndef SDLW_VIDEO_INCLUDED
#define SDLW_VIDEO_INCLUDED

#include <array>

#include <SDL_video.h>

#include <sdlw/error.hpp>
#include <sdlw/pixels.hpp>
#include <sdlw/rect.hpp>
#include <sdlw/surface.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

struct display_mode {
    u32 format; // pixel_format_type
    size size;
    int refresh_rate;
    void* driver_data;
};

class renderer_ref; // for window::renderer

// clang-format off

using window_id = u32;

enum class window_fullscreen_mode : u32 {
    normal          = SDL_WINDOW_FULLSCREEN,
    fake_fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
    windowed        = 0
};

enum class hit_test_result {
    normal              = SDL_HITTEST_NORMAL,
    draggable           = SDL_HITTEST_DRAGGABLE,
    resize_top_left     = SDL_HITTEST_RESIZE_TOPLEFT,
    resize_top          = SDL_HITTEST_RESIZE_TOP,
    resize_top_right    = SDL_HITTEST_RESIZE_TOPRIGHT,
    resize_right        = SDL_HITTEST_RESIZE_RIGHT,
    resize_bottom_right = SDL_HITTEST_RESIZE_BOTTOMRIGHT,
    resize_bottom       = SDL_HITTEST_RESIZE_BOTTOM,
    resize_bottom_left  = SDL_HITTEST_RESIZE_BOTTOMLEFT,
    resize_left         = SDL_HITTEST_RESIZE_LEFT
};

// clang-format on

struct borders { int top, left, bottom, right; };

struct window_ref;

class window {
protected:
    SDL_Window* _handle = nullptr;

public:
    ~window()
    {
        if (_handle) SDL_DestroyWindow(_handle);
    }

    window() = default;

    window(window&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    window& operator=(window&& other) noexcept
    {
        if (_handle) SDL_DestroyWindow(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit window(SDL_Window* handle) noexcept
        : _handle{handle}
    {}

    SDL_Window* handle() const noexcept
    {
        return _handle;
    }

    window(window_ref) = delete;

    enum flags : u32 {
        fullscreen         = SDL_WINDOW_FULLSCREEN,
        opengl             = SDL_WINDOW_OPENGL,
        shown              = SDL_WINDOW_SHOWN,
        hidden             = SDL_WINDOW_HIDDEN,
        borderless         = SDL_WINDOW_BORDERLESS,
        resizable          = SDL_WINDOW_RESIZABLE,
        minimized          = SDL_WINDOW_MINIMIZED,
        maximized          = SDL_WINDOW_MAXIMIZED,
        input_grabbed      = SDL_WINDOW_INPUT_GRABBED,
        input_focus        = SDL_WINDOW_INPUT_FOCUS,
        mouse_focus        = SDL_WINDOW_MOUSE_FOCUS,
        fullscreen_desktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
        foreign            = SDL_WINDOW_FOREIGN,
        allow_high_dpi     = SDL_WINDOW_ALLOW_HIGHDPI,
        mouse_capture      = SDL_WINDOW_MOUSE_CAPTURE,
        always_on_top      = SDL_WINDOW_ALWAYS_ON_TOP,
        skip_taskbar       = SDL_WINDOW_SKIP_TASKBAR,
        utility            = SDL_WINDOW_UTILITY,
        tooltip            = SDL_WINDOW_TOOLTIP,
        popup_menu         = SDL_WINDOW_POPUP_MENU,
        vulkan             = SDL_WINDOW_VULKAN
    };

    // clang-format off
    static constexpr auto centered      = int{SDL_WINDOWPOS_CENTERED};
    static constexpr auto undefined     = int{SDL_WINDOWPOS_UNDEFINED};
    static constexpr auto pos_centered  = point{centered, centered};
    static constexpr auto pos_undefined = point{undefined, undefined};
    // clang-format on

    static constexpr auto centered_on_display(int display_index) noexcept -> int
    {
        return SDL_WINDOWPOS_CENTERED_DISPLAY(display_index);
    }

    static constexpr auto undefined_on_display(int display_index) noexcept -> int
    {
        return SDL_WINDOWPOS_UNDEFINED_DISPLAY(display_index);
    }

    static constexpr auto pos_centered_on_display(int display_index) noexcept -> point
    {
        const auto value = centered_on_display(display_index);
        return point{value, value};
    }

    static constexpr auto pos_undefined_on_display(int display_index) noexcept -> point
    {
        const auto value = undefined_on_display(display_index);
        return point{value, value};
    }

    static constexpr auto is_centered(int coordinate) noexcept -> bool
    {
        return SDL_WINDOWPOS_ISCENTERED(coordinate);
    }

    static constexpr auto is_undefined(int coordinate) noexcept -> bool
    {
        return SDL_WINDOWPOS_ISUNDEFINED(coordinate);
    }

    static constexpr auto is_pos_centered(const point& position) noexcept -> bool
    {
        return is_centered(position.x) && is_centered(position.y);
    }

    static constexpr auto is_pos_undefined(const point& position) noexcept -> bool
    {
        return is_undefined(position.x) && is_undefined(position.y);
    }

    window(const char* title, const rect& bounds, u32 flags)
        : _handle{SDL_CreateWindow(title, bounds.position.x, bounds.position.y, bounds.size.width, bounds.size.height, flags)}
    {
        if (!_handle) throw error{};
    }

    explicit window(void* native_window_data)
        : _handle{SDL_CreateWindowFrom(native_window_data)}
    {
        if (!_handle) throw error{};
    }

    u32 flags() const noexcept
    {
        return SDL_GetWindowFlags(_handle);
    }

    window_id id() const noexcept
    {
        return static_cast<window_id>(SDL_GetWindowID(_handle));
    }

    const char* title() const noexcept
    {
        return SDL_GetWindowTitle(_handle);
    }

    void set_title(const char* title) noexcept
    {
        SDL_SetWindowTitle(_handle, title);
    }

    point position() const noexcept
    {
        int x, y;
        SDL_GetWindowPosition(_handle, &x, &y);
        return point{x, y};
    }

    void set_position(const point& position) noexcept
    {
        SDL_SetWindowPosition(_handle, position.x, position.y);
    }

    sdl::size size() const noexcept
    {
        int width, height;
        SDL_GetWindowSize(_handle, &width, &height);
        return {width, height};
    }

    void set_size(const sdl::size& size) noexcept
    {
        SDL_SetWindowSize(_handle, size.width, size.height);
    }

    void set_resizable(bool resizable) noexcept
    {
        SDL_SetWindowResizable(_handle, static_cast<SDL_bool>(resizable));
    }

    borders border_sizes() const
    {
        borders b;
        if (SDL_GetWindowBordersSize(_handle, &b.top, &b.left, &b.bottom, &b.right) < 0) {
            throw error{};
        }
        return b;
    }

    void set_bordered(bool bordered) noexcept
    {
        SDL_SetWindowBordered(_handle, static_cast<SDL_bool>(bordered));
    }

    sdl::size max_size() const noexcept
    {
        sdl::size s;
        SDL_GetWindowMaximumSize(_handle, &s.width, &s.height);
        return s;
    }

    void set_max_size(const sdl::size& size) noexcept
    {
        SDL_SetWindowMaximumSize(_handle, size.width, size.height);
    }

    sdl::size min_size() const noexcept
    {
        sdl::size s;
        SDL_GetWindowMinimumSize(_handle, &s.width, &s.height);
        return s;
    }

    void set_min_size(const sdl::size& size) noexcept
    {
        SDL_SetWindowMinimumSize(_handle, size.width, size.height);
    }

    bool is_input_grabbed() const noexcept
    {
        return static_cast<bool>(SDL_GetWindowGrab(_handle));
    }

    void set_input_grab(bool is_grabbed) noexcept
    {
        SDL_SetWindowGrab(_handle, static_cast<SDL_bool>(is_grabbed));
    }

    void* data(const char* name) const noexcept
    {
        return SDL_GetWindowData(_handle, name);
    }

    void* set_data(const char* name, void* user_data) noexcept
    {
        return SDL_SetWindowData(_handle, name, user_data);
    }

    surface_ref surface()
    {
        SDL_Surface* handle = SDL_GetWindowSurface(_handle);
        if (!handle) throw error{};
        return surface_ref{handle};
    }

    float opacity() const
    {
        float opacity;
        if (SDL_GetWindowOpacity(_handle, &opacity) != 0) {
            throw error{};
        }
        return opacity;
    }

    void set_opacity(float opacity)
    {
        if (SDL_SetWindowOpacity(_handle, opacity) == 0) {
            throw error{};
        }
    }

    sdl::display_mode display_mode() const
    {
        sdl::display_mode mode;
        if (SDL_GetWindowDisplayMode(_handle, reinterpret_cast<SDL_DisplayMode*>(&mode)) != 0) {
            throw error{};
        }
        return mode;
    }

    void set_display_mode(const sdl::display_mode* mode)
    {
        const auto pmode = reinterpret_cast<const SDL_DisplayMode*>(mode);
        if (SDL_SetWindowDisplayMode(_handle, pmode) < 0) {
            throw error{};
        }
    }

    int display_index() const
    {
        int index = SDL_GetWindowDisplayIndex(_handle);
        if (index < 0) throw error{};
        return index;
    }

    sdl::pixel_format_type pixel_format_type() const
    {
        u32 format = SDL_GetWindowPixelFormat(_handle);
        if (format == SDL_PIXELFORMAT_UNKNOWN) {
            throw error{};
        }
        return static_cast<sdl::pixel_format_type>(format);
    }

    void hide() noexcept
    {
        SDL_HideWindow(_handle);
    }

    void show() noexcept
    {
        SDL_ShowWindow(_handle);
    }

    void set_fullscreen_mode(window_fullscreen_mode mode)
    {
        if (SDL_SetWindowFullscreen(_handle, static_cast<u32>(mode)) < 0) {
            throw error{};
        }
    }

    void maximize() noexcept
    {
        SDL_MaximizeWindow(_handle);
    }

    void minimize() noexcept
    {
        SDL_MinimizeWindow(_handle);
    }

    void restore() noexcept
    {
        SDL_RestoreWindow(_handle);
    }

    void raise() noexcept
    {
        SDL_RaiseWindow(_handle);
    }

    void set_input_focus()
    {
        if (SDL_SetWindowInputFocus(_handle) < 0) {
            throw error{};
        }
    }

    void update_surface()
    {
        if (SDL_UpdateWindowSurface(_handle) < 0) {
            throw error{};
        }
    }

    void update_surface_areas(span<const rect> areas)
    {
        auto sdl_areas = reinterpret_cast<const SDL_Rect*>(areas.data());
        auto sdl_areas_size = static_cast<int>(areas.size());
        if (SDL_UpdateWindowSurfaceRects(_handle, sdl_areas, sdl_areas_size) < 0) {
            throw error{};
        }
    }

    void set_icon(sdl::surface_ref icon) noexcept
    {
        SDL_SetWindowIcon(_handle, icon.handle());
    }

    auto renderer() -> renderer_ref;

    void set_modal(const window& modal)
    {
        if (SDL_SetWindowModalFor(modal.handle(), _handle) < 0) {
            throw error{};
        }
    }

    template<typename HitTest>
    void set_hit_test(HitTest& ht);

    void set_hit_test(hit_test_result (*ht)(window_ref, const point&));

    static window_ref from(window_id) noexcept;

    static window_ref grabbed() noexcept;

    friend bool operator==(const window& x, const window& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const window& x, const window& y) noexcept
    {
        return !(x == y);
    }
};

struct window_ref : window {
    ~window_ref()
    {
        _handle = nullptr;
    }

    window_ref(const window_ref& other) noexcept
        : window{other._handle}
    {}

    window_ref(const window& other) noexcept
        : window{other.handle()}
    {}

    explicit window_ref(SDL_Window* handle) noexcept
        : window{handle}
    {}

    SDL_Window* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

template<typename HitTest>
void window::set_hit_test(HitTest& ht)
{
    static_assert(std::is_invocable_r_v<hit_test_result, HitTest, window_ref, const point&>);
    constexpr auto sdl_callback = [](SDL_Window* win, const SDL_Point* area, void* data) -> SDL_HitTestResult {
        auto& test = *static_cast<HitTest*>(data);
        const auto result = test(window_ref{win}, *area);
        return static_cast<SDL_HitTestResult>(result);
    };
    if (SDL_SetWindowHitTest(_handle, sdl_callback, &ht) < 0) {
        throw error{};
    }
}

inline void window::set_hit_test(hit_test_result (*ht)(window_ref, const point&))
{
    auto fp_sdl_callback = [](SDL_Window* win, const SDL_Point* area, void* data) -> SDL_HitTestResult {
        auto test = reinterpret_cast<hit_test_result (*)(window_ref, const point&)>(data);
        const auto result = test(window_ref{win}, *reinterpret_cast<const point*>(area));
        return static_cast<SDL_HitTestResult>(result);
    };
    if (SDL_SetWindowHitTest(_handle, fp_sdl_callback, reinterpret_cast<void*>(ht)) < 0) {
        throw error{};
    }
}

inline window_ref window::from(window_id id) noexcept
{
    return window_ref{SDL_GetWindowFromID(static_cast<u32>(id))};
}

inline window_ref window::grabbed() noexcept
{
    return window_ref{SDL_GetGrabbedWindow()};
}

struct dpi { float diagonal, horizontal, vertical; };

struct display {
    int index;

    display_mode closest_mode(const display_mode& m) const
    {
        const auto pmode = reinterpret_cast<const SDL_DisplayMode*>(&m);
        sdl::display_mode closest;
        if (SDL_GetClosestDisplayMode(index, pmode, reinterpret_cast<SDL_DisplayMode*>(&closest)) == nullptr) {
            throw error{};
        }
        return closest;
    }

    display_mode current_mode() const
    {
        sdl::display_mode current;
        if (SDL_GetCurrentDisplayMode(index, reinterpret_cast<SDL_DisplayMode*>(&current)) != 0) {
            throw error{};
        }
        return current;
    }

    display_mode desktop_mode() const
    {
        sdl::display_mode desktop;
        if (SDL_GetDesktopDisplayMode(index, reinterpret_cast<SDL_DisplayMode*>(&desktop)) != 0) {
            throw error{};
        }
        return desktop;
    }

    rect bounds() const
    {
        SDL_Rect bounds;
        if (SDL_GetDisplayBounds(index, &bounds) != 0) {
            throw error{};
        }
        return {bounds.x, bounds.y, bounds.w, bounds.h};
    }

    sdl::dpi dpi() const
    {
        float ddpi, hdpi, vdpi;
        if (SDL_GetDisplayDPI(index, &ddpi, &hdpi, &vdpi) != 0) {
            throw error{};
        }
        return {ddpi, hdpi, vdpi};
    }

    const char* name() const noexcept
    {
        return SDL_GetDisplayName(index);
    }

    rect usable_bounds() const
    {
        SDL_Rect r;
        if (SDL_GetDisplayUsableBounds(index, &r) != 0) {
            throw error{};
        }
        return {r.x, r.y, r.w, r.h};
    }

    display_mode mode(int mode_index) const
    {
        sdl::display_mode mode;
        if (SDL_GetDisplayMode(index, mode_index, reinterpret_cast<SDL_DisplayMode*>(&mode)) != 0) {
            throw error{};
        }
        return mode;
    }

    int num_modes() const
    {
        int count = SDL_GetNumDisplayModes(index);
        if (count < 0) throw error{};
        return count;
    }

    static int count()
    {
        int result = SDL_GetNumVideoDisplays();
        if (result < 0) throw error{};
        return result;
    }

    static float brightness(const window& win) noexcept
    {
        return SDL_GetWindowBrightness(win.handle());
    }

    static void set_brightness(const window& win, float brightness)
    {
        if (SDL_SetWindowBrightness(win.handle(), brightness) < 0) {
            throw error{};
        }
    }

    static void gamma_ramp(const window& win, span<u16, int> red, span<u16, int> green, span<u16, int> blue)
    {
        if (SDL_GetWindowGammaRamp(win.handle(), red.data(), green.data(), blue.data()) < 0) {
            throw error{};
        }
    }

    static void set_gamma_ramp(const window& win, span<const u16, int> red, span<const u16, int> green, span<const u16, int> blue)
    {
        if (SDL_SetWindowGammaRamp(win.handle(), red.data(), green.data(), blue.data()) < 0) {
            throw error{};
        }
    }
};

struct screen_saver {
    static bool enabled() noexcept
    {
        return SDL_IsScreenSaverEnabled();
    }

    static void enable() noexcept
    {
        SDL_EnableScreenSaver();
    }

    static void disable() noexcept
    {
        SDL_DisableScreenSaver();
    }
};

struct video_driver {
    int index;

    const char* name() const noexcept
    {
        return SDL_GetVideoDriver(index);
    }


    static int count() noexcept
    {
        return SDL_GetNumVideoDrivers();
    }

    static const char* current() noexcept
    {
        return SDL_GetCurrentVideoDriver();
    }
};

struct video_subsystem {
    video_subsystem(const char* driver_name)
    {
        if (SDL_VideoInit(driver_name) < 0) {
            throw error{};
        }
    }

    ~video_subsystem()
    {
        SDL_VideoQuit();
    }
};

namespace gl {

struct context_ref;

class context {
protected:
    SDL_GLContext _handle = nullptr;

public:
    ~context()
    {
        if (_handle) SDL_GL_DeleteContext(_handle);
    }

    context() = default;

    context(context&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    context& operator=(context&& other) noexcept
    {
        if (_handle) SDL_GL_DeleteContext(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit context(SDL_GLContext handle) noexcept
        : _handle{handle}
    {}

    SDL_GLContext handle() const noexcept
    {
        return _handle;
    }

    context(context_ref) = delete;

    explicit context(window& w)
        : context{SDL_GL_CreateContext(w.handle())}
    {
        if (!_handle) throw error{};
    }

    friend bool operator==(const context& x, const context& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const context& x, const context& y) noexcept
    {
        return !(x == y);
    }
};

struct context_ref : context {
    ~context_ref()
    {
        _handle = nullptr;
    }

    context_ref(const context_ref& other) noexcept
        : context{other._handle}
    {}

    context_ref(const context& other) noexcept
        : context{other.handle()}
    {}

    explicit context_ref(SDL_GLContext handle) noexcept
        : context{handle}
    {}

    SDL_GLContext handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

enum class attr : std::underlying_type_t<SDL_GLattr> {
    red_size                   = SDL_GL_RED_SIZE,
    green_size                 = SDL_GL_GREEN_SIZE,
    blue_size                  = SDL_GL_BLUE_SIZE,
    alpha_size                 = SDL_GL_ALPHA_SIZE,
    buffer_size                = SDL_GL_BUFFER_SIZE,
    double_buffer              = SDL_GL_DOUBLEBUFFER,
    depth_size                 = SDL_GL_DEPTH_SIZE,
    stencil_size               = SDL_GL_STENCIL_SIZE,
    accum_red_size             = SDL_GL_ACCUM_RED_SIZE,
    accum_green_size           = SDL_GL_ACCUM_GREEN_SIZE,
    accum_blue_size            = SDL_GL_ACCUM_BLUE_SIZE,
    accum_alpha_size           = SDL_GL_ACCUM_ALPHA_SIZE,
    stereo                     = SDL_GL_STEREO,
    multisample_buffers        = SDL_GL_MULTISAMPLEBUFFERS,
    multisample_samples        = SDL_GL_MULTISAMPLESAMPLES,
    accelerated_visual         = SDL_GL_ACCELERATED_VISUAL,
    retained_backing           = SDL_GL_RETAINED_BACKING,
    context_major_version      = SDL_GL_CONTEXT_MAJOR_VERSION,
    context_minor_version      = SDL_GL_CONTEXT_MINOR_VERSION,
    context_egl                = SDL_GL_CONTEXT_EGL,
    context_flags              = SDL_GL_CONTEXT_FLAGS,
    context_profile_mask       = SDL_GL_CONTEXT_PROFILE_MASK,
    share_with_current_context = SDL_GL_SHARE_WITH_CURRENT_CONTEXT,
    framebuffer_srgb_capable   = SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,
    context_release_behavior   = SDL_GL_CONTEXT_RELEASE_BEHAVIOR,
    context_reset_notification = SDL_GL_CONTEXT_RESET_NOTIFICATION,
    context_no_error           = SDL_GL_CONTEXT_NO_ERROR,
};

enum class profile : std::underlying_type_t<SDL_GLprofile> {
    core          = SDL_GL_CONTEXT_PROFILE_CORE,
    compatibility = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
    es            = SDL_GL_CONTEXT_PROFILE_ES
};

enum class context_flag : std::underlying_type_t<SDL_GLcontextFlag> {
    debug              = SDL_GL_CONTEXT_DEBUG_FLAG,
    forward_compatible = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
    robust_access      = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
    reset_isolation    = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
};

inline bool extension_supported(const char* extension) noexcept
{
    return static_cast<bool>(SDL_GL_ExtensionSupported(extension));
}

inline int attribute(attr a)
{
    auto result = int{};
    if (SDL_GL_GetAttribute(static_cast<SDL_GLattr>(a), &result) < 0) {
        throw error{};
    } else {
        return result;
    }
}

inline context_ref current_context()
{
    SDL_GLContext handle = SDL_GL_GetCurrentContext();
    if (!handle) throw error{};
    return context_ref{handle};
}

inline window_ref current_window()
{
    SDL_Window* handle = SDL_GL_GetCurrentWindow();
    if (!handle) throw error{};
    return window_ref{handle};
}

inline sdl::size drawable_size(const window& w) noexcept
{
    sdl::size s;
    SDL_GL_GetDrawableSize(w.handle(), &s.width, &s.height);
    return s;
}

inline void* proc_address(const char* proc) noexcept
{
    return SDL_GL_GetProcAddress(proc);
}

inline int swap_interval() noexcept
{
    return SDL_GL_GetSwapInterval();
}

inline void load_library(const char* path)
{
    if (SDL_GL_LoadLibrary(path) < 0) {
        throw error{};
    }
}

inline void make_current(const window& w, context_ref c)
{
    if (SDL_GL_MakeCurrent(w.handle(), c.handle()) < 0) {
        throw error{};
    }
}

inline void reset_attributes() noexcept
{
    SDL_GL_ResetAttributes();
}

template<typename IntegerOrEnum>
void set_attribute(attr a, IntegerOrEnum value)
{
    if (SDL_GL_SetAttribute(static_cast<SDL_GLattr>(a), static_cast<int>(value)) < 0) {
        throw error{};
    }
}

inline void set_swap_interval(int interval)
{
    if (SDL_GL_SetSwapInterval(interval) < 0) {
        throw error{};
    }
}

inline void swap_window(const window& w) noexcept
{
    return SDL_GL_SwapWindow(w.handle());
}

inline void unload_library() noexcept
{
    SDL_GL_UnloadLibrary();
}

} // namespace gl

} // namespace sdl

#endif // SDLW_VIDEO_INCLUDED
