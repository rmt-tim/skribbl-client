#ifndef SDLW_MOUSE_INCLUDED
#define SDLW_MOUSE_INCLUDED

#include <SDL_mouse.h>

#include <sdlw/error.hpp>
#include <sdlw/rect.hpp>
#include <sdlw/video.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

// clang-format off

enum class system_cursor {
    arrow              = SDL_SYSTEM_CURSOR_ARROW,
    ibeam              = SDL_SYSTEM_CURSOR_IBEAM,
    wait               = SDL_SYSTEM_CURSOR_WAIT,
    crosshair          = SDL_SYSTEM_CURSOR_CROSSHAIR,
    wait_arrow         = SDL_SYSTEM_CURSOR_WAITARROW,
    size_nwse          = SDL_SYSTEM_CURSOR_SIZENWSE,
    size_nesw          = SDL_SYSTEM_CURSOR_SIZENESW,
    size_we            = SDL_SYSTEM_CURSOR_SIZEWE,
    size_ns            = SDL_SYSTEM_CURSOR_SIZENS,
    size_all           = SDL_SYSTEM_CURSOR_SIZEALL,
    no                 = SDL_SYSTEM_CURSOR_NO,
    hand               = SDL_SYSTEM_CURSOR_HAND,
    num_system_cursors = SDL_NUM_SYSTEM_CURSORS
};

// clang-format on

struct cursor_ref;

class cursor {
protected:
    SDL_Cursor* _handle = nullptr;

public:
    ~cursor()
    {
        if (_handle) SDL_FreeCursor(_handle);
    }

    cursor() = default;

    cursor(cursor&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    cursor& operator=(cursor&& other) noexcept
    {
        if (_handle) SDL_FreeCursor(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit cursor(SDL_Cursor* handle) noexcept
        : _handle{handle}
    {}

    SDL_Cursor* handle() const noexcept
    {
        return _handle;
    }

    cursor(cursor_ref) = delete;

    cursor(const u8* data, const u8* mask, const size& sz, const point& topleft_corner)
        : cursor{SDL_CreateCursor(data, mask, sz.width, sz.height, topleft_corner.x, topleft_corner.y)}
    {
        if (!_handle) throw error{};
    }

    cursor(const surface& surf, const point& topleft_corner)
        : cursor{SDL_CreateColorCursor(surf.handle(), topleft_corner.x, topleft_corner.y)}
    {
        if (!_handle) throw error{};
    }

    explicit cursor(system_cursor sysc)
        : cursor{SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(sysc))}
    {
        if (!_handle) throw error{};
    }

    friend bool operator==(const cursor& x, const cursor& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const cursor& x, const cursor& y) noexcept
    {
        return !(x == y);
    }
};

struct cursor_ref : cursor {
    ~cursor_ref()
    {
        _handle = nullptr;
    }

    cursor_ref(const cursor_ref& other) noexcept
        : cursor{other._handle}
    {}

    cursor_ref(const cursor& other) noexcept
        : cursor{other.handle()}
    {}

    explicit cursor_ref(SDL_Cursor* handle) noexcept
        : cursor{handle}
    {}

    SDL_Cursor* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

struct active_cursor {
    static cursor_ref get() noexcept
    {
        return cursor_ref{SDL_GetCursor()};
    }

    static void set(cursor& c) noexcept
    {
        SDL_SetCursor(c.handle());
    }

    static void redraw() noexcept
    {
        SDL_SetCursor(nullptr);
    }

    static bool shown()
    {
        int result = SDL_ShowCursor(-1);
        if (result < 0) throw error{};
        return static_cast<bool>(result);
    }

    static void show()
    {
        int result = SDL_ShowCursor(1);
        if (result < 0) throw error{};
    }

    static void hide()
    {
        int result = SDL_ShowCursor(0);
        if (result < 0) throw error{};
    }
};

// clang-format off

enum class mouse_button : u8 {
    left   = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right  = SDL_BUTTON_RIGHT,
    x1     = SDL_BUTTON_X1,
    x2     = SDL_BUTTON_X2
};

// TODO: flags? naming? mouse_button and mouse_button_state both exist.
enum class mouse_button_state : u32 {
    left   = SDL_BUTTON_LMASK,
    middle = SDL_BUTTON_MMASK,
    right  = SDL_BUTTON_RMASK,
    x1     = SDL_BUTTON_X1MASK,
    x2     = SDL_BUTTON_X2MASK
};

enum class mouse_wheel_direction : u32 {
    normal  = SDL_MOUSEWHEEL_NORMAL,
    flipped = SDL_MOUSEWHEEL_FLIPPED
};

/* SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(mouse_button_state); */

// clang-format on

constexpr mouse_button_state button_mask(mouse_button b) noexcept
{
    return static_cast<mouse_button_state>(SDL_BUTTON(static_cast<u8>(b)));
}

inline std::pair<mouse_button_state, point> state() noexcept
{
    point pos;
    const auto sdl_button_state = SDL_GetMouseState(&pos.x, &pos.y);
    const auto bstate = static_cast<mouse_button_state>(sdl_button_state);
    return {bstate, pos};
}

inline std::pair<mouse_button_state, point> global_state() noexcept
{
    auto pos = point{};
    const auto sdl_button_state = SDL_GetMouseState(&pos.x, &pos.y);
    const auto bstate = static_cast<mouse_button_state>(sdl_button_state);
    return {bstate, pos};
}

inline window_ref mouse_focus() noexcept
{
    return window_ref{SDL_GetMouseFocus()};
}

inline bool capture_mouse(bool enabled) noexcept
{
    return SDL_CaptureMouse(enabled ? SDL_TRUE : SDL_FALSE) == 0;
}

inline void warp_mouse_in_window(const window& win, const point& p) noexcept
{
    SDL_WarpMouseInWindow(win.handle(), p.x, p.y);
}

inline bool warp_mouse_global(const point& coordinate) noexcept
{
    return SDL_WarpMouseGlobal(coordinate.x, coordinate.y) == 0;
}

struct relative_mouse_mode {
    static bool is_enabled() noexcept
    {
        return SDL_GetRelativeMouseMode();
    }

    static void enable()
    {
        if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
            throw error{};
        }
    }

    static void disable()
    {
        if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0) {
            throw error{};
        }
    }

    static std::pair<mouse_button_state, point> state() noexcept
    {
        auto p = point{};
        const auto sdl_button_state = SDL_GetRelativeMouseState(&p.x, &p.y);
        const auto bstate = static_cast<mouse_button_state>(sdl_button_state);
        return {bstate, p};
    }
};

} // namespace sdl

#endif // SDLW_MOUSE_INCLUDED
