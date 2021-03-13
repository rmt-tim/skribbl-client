#ifndef SDLW_KEYBOARD_INCLUDED
#define SDLW_KEYBOARD_INCLUDED

#include <SDL_keyboard.h>

#include <sdlw/keycode.hpp>
#include <sdlw/scancode.hpp>
#include <sdlw/video.hpp>

namespace sdl {

struct key {
    scancode scancode;
    keycode sym;
    u16 mod;
    u32 unused;
};

inline keycode to_keycode(const char* name)
{
    SDL_Keycode result = SDL_GetKeyFromName(name);
    if (result == SDLK_UNKNOWN) throw error{};
    return static_cast<keycode>(result);
}

inline keycode to_keycode(scancode sc) noexcept
{
    const auto sdl_scancode = static_cast<SDL_Scancode>(sc);
    const auto sdl_keycode = SDL_GetKeyFromScancode(sdl_scancode);
    return static_cast<keycode>(sdl_keycode);
}

inline const char* name(keycode kc) noexcept
{
    return SDL_GetKeyName(static_cast<SDL_Keycode>(kc));
}

inline scancode to_scancode(const char* name)
{
    const auto result = SDL_GetScancodeFromName(name);
    if (result == SDL_SCANCODE_UNKNOWN) {
        throw error{};
    } else {
        return static_cast<scancode>(result);
    }
}

inline scancode to_scancode(keycode kc) noexcept
{
    const auto sdl_keycode = static_cast<SDL_Keycode>(kc);
    const auto sdl_scancode = SDL_GetScancodeFromKey(sdl_keycode);
    return static_cast<scancode>(sdl_scancode);
}

inline const char* name(scancode sc) noexcept
{
    return SDL_GetScancodeName(static_cast<SDL_Scancode>(sc));
}

inline window_ref keyboard_focus() noexcept
{
    return window_ref{SDL_GetKeyboardFocus()};
}

inline bool is_pressed(scancode sc) noexcept
{
    return SDL_GetKeyboardState(nullptr)[static_cast<SDL_Scancode>(sc)];
}

inline keymod mod_state() noexcept
{
    return static_cast<keymod>(SDL_GetModState());
}

inline void set_mod_state(keymod mod) noexcept
{
    SDL_SetModState(static_cast<SDL_Keymod>(mod));
}

struct text_input {
    static bool is_active() noexcept
    {
        return SDL_IsTextInputActive();
    }

    static void start() noexcept
    {
        SDL_StartTextInput();
    }

    static void stop() noexcept
    {
        SDL_StopTextInput();
    }

    static void set_candidate_list_bounds(const rect& bounds) noexcept
    {
        auto r = reinterpret_cast<SDL_Rect*>(const_cast<rect*>(&bounds));
        SDL_SetTextInputRect(r);
    }
};

struct screen_keyboard {
    static bool is_supported() noexcept
    {
        return SDL_HasScreenKeyboardSupport();
    }

    static bool is_shown(const window& win) noexcept
    {
        return SDL_IsScreenKeyboardShown(win.handle());
    }
};

} // namespace sdl

#endif // SDLW_KEYBOARD_INCLUDED
