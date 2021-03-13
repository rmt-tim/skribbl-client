#ifndef SDLW_MESSAGE_BOX_INCLUDED
#define SDLW_MESSAGE_BOX_INCLUDED

#include <SDL_messagebox.h>

#include <sdlw/video.hpp>

namespace sdl {

class message_box {
public:
    // clang-format off
    enum flags : u32 {
        error       = SDL_MESSAGEBOX_ERROR,
        warning     = SDL_MESSAGEBOX_WARNING,
        information = SDL_MESSAGEBOX_INFORMATION
    };
    // clang-format on
};

inline void show_simple_message_box(message_box::flags f, const char* title, const char* message)
{
    if (SDL_ShowSimpleMessageBox(static_cast<u32>(f), title, message, nullptr) < 0) {
        throw error{};
    }
}

inline void show_simple_message_box(message_box::flags f, const char* title, const char* message, const window& parent)
{
    if (SDL_ShowSimpleMessageBox(static_cast<u32>(f), title, message, parent.handle()) < 0) {
        throw error{};
    }
}

} // namespace sdl

#endif // SDLW_MESSAGE_BOX_INCLUDED
