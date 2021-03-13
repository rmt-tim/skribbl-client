#ifndef SDLW_TOUCH_INCLUDED
#define SDLW_TOUCH_INCLUDED

#include <SDL_touch.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>

namespace sdl {

using finger_id = SDL_FingerID;

using finger = SDL_Finger;

using touch_id = SDL_TouchID;

static const touch_id mouse_id = SDL_TOUCH_MOUSEID;

struct touch {
    touch_id id;

    int num_fingers() const
    {
        int result = SDL_GetNumTouchFingers(static_cast<SDL_TouchID>(id));
        if (result == 0) throw error{};
        return result;
    }

    sdl::finger& finger(int finger_index) const
    {
        SDL_Finger* result = SDL_GetTouchFinger(id, finger_index);
        if (!result) throw error{};
        return *result;
    }
};

struct touch_device {
    int index;

    sdl::touch touch() const
    {
        touch_id result = SDL_GetTouchDevice(index);
        if (result == 0) throw error{};
        return {result};
    }

    static int count() noexcept
    {
        return SDL_GetNumTouchDevices();
    }
};

} // namespace sdl

#endif // SDLW_TOUCH_INCLUDED
