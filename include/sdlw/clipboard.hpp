#ifndef SDLW_CLIPBOARD_INCLUDED
#define SDLW_CLIPBOARD_INCLUDED

#include <SDL_clipboard.h>

#include <sdlw/error.hpp>

namespace sdl {

struct clipboard {
    static bool has_text() noexcept
    {
        return SDL_HasClipboardText();
    }

    static const char* text() noexcept
    {
        return SDL_GetClipboardText();
    }

    static void set_text(const char* text)
    {
        if (SDL_SetClipboardText(text) < 0) {
            throw error{};
        }
    }
};

} // namespace sdl

#endif // SDLW_CLIPBOARD_INCLUDED
