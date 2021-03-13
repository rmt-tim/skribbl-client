#ifndef SDLW_JOYSTICK_INCLUDED
#define SDLW_JOYSTICK_INCLUDED

#include <cstring>

#include <SDL_events.h>
#include <SDL_joystick.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

using joystick_id = SDL_JoystickID;

struct joystick_guid {
    u8 data[16];

    joystick_guid() = default;

    joystick_guid(const char* pch_guid) noexcept
    {
        SDL_JoystickGUID guid = SDL_JoystickGetGUIDFromString(pch_guid);
        std::memcpy(data, guid.data, 16);
    }

    void string(span<char, int> psz_guid) const noexcept
    {
        SDL_JoystickGUID guid;
        std::memcpy(&guid, data, 16);
        SDL_JoystickGetGUIDString(guid, psz_guid.data(), psz_guid.size());
    }
};

// clang-format off

enum class joystick_hat_position : u8 {
    centered   = SDL_HAT_CENTERED,
    up         = SDL_HAT_UP,
    right      = SDL_HAT_RIGHT,
    down       = SDL_HAT_DOWN,
    left       = SDL_HAT_LEFT,
    right_up   = SDL_HAT_RIGHTUP,
    right_down = SDL_HAT_RIGHTDOWN,
    left_up    = SDL_HAT_LEFTUP,
    left_down  = SDL_HAT_LEFTDOWN
};

enum class joystick_power_level : std::underlying_type_t<SDL_JoystickPowerLevel> {
    unknown = SDL_JOYSTICK_POWER_UNKNOWN,
    empty   = SDL_JOYSTICK_POWER_EMPTY,
    low     = SDL_JOYSTICK_POWER_LOW,
    medium  = SDL_JOYSTICK_POWER_MEDIUM,
    full    = SDL_JOYSTICK_POWER_FULL,
    wired   = SDL_JOYSTICK_POWER_WIRED,
    max     = SDL_JOYSTICK_POWER_MAX
};

struct joystick_device {
    int index;

    static int count()
    {
        int result = SDL_NumJoysticks();
        if (result < 0) throw error{};
        return result;
    }

    joystick_guid guid() const noexcept
    {
        SDL_JoystickGUID result = SDL_JoystickGetDeviceGUID(index);
        joystick_guid guid;
        std::memcpy(&guid, result.data, 16);
        return guid;
    }

    const char* name() const
    {
        const char* s = SDL_JoystickNameForIndex(index);
        if (!s) throw error{};
        return s;
    }
};

struct joystick_ref;

class joystick {
protected:
    SDL_Joystick* _handle = nullptr;

public:
    ~joystick()
    {
        if (_handle) SDL_JoystickClose(_handle);
    }

    joystick() = default;

    joystick(joystick&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    joystick& operator=(joystick&& other) noexcept
    {
        if (_handle) SDL_JoystickClose(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit joystick(SDL_Joystick* handle) noexcept
        : _handle{handle}
    {}

    SDL_Joystick* handle() const noexcept
    {
        return _handle;
    }

    joystick(joystick_ref) = delete;

    explicit joystick(int device_index)
        : _handle{SDL_JoystickOpen(device_index)}
    {
        if (!_handle) throw error{};
    }

    joystick_power_level power_level() const
    {
        SDL_JoystickPowerLevel pl = SDL_JoystickCurrentPowerLevel(_handle);
        if (pl == SDL_JOYSTICK_POWER_UNKNOWN) throw error{};
        return static_cast<joystick_power_level>(pl);
    }

    bool attached() const noexcept
    {
        return static_cast<bool>(SDL_JoystickGetAttached(_handle));
    }

    i16 axis(int axis) const noexcept
    {
        i16 result = SDL_JoystickGetAxis(_handle, axis);
        return result;
    }

    std::pair<int, int> ball(int ball) const
    {
        int dx;
        int dy;
        if (SDL_JoystickGetBall(_handle, ball, &dx, &dy) < 0) {
            throw error{};
        }
        return {dx, dy};
    }

    bool button(int button) const
    {
        return static_cast<bool>(SDL_JoystickGetButton(_handle, button));
    }

    joystick_guid guid() const noexcept
    {
        const auto result = SDL_JoystickGetGUID(_handle);
        joystick_guid guid;
        std::memcpy(&guid, result.data, 16);
        return guid;
    }

    joystick_hat_position hat(int hat) const noexcept
    {
        return static_cast<joystick_hat_position>(SDL_JoystickGetHat(_handle, hat));
    }

    joystick_id id() const
    {
        SDL_JoystickID result = SDL_JoystickInstanceID(_handle);
        if (result < 0) throw error{};
        return result;
    }

    const char* name() const
    {
        const char* s = SDL_JoystickName(_handle);
        if (!s) throw error{};
        return s;
    }

    int num_axes() const
    {
        int result = SDL_JoystickNumAxes(_handle);
        if (result < 0) throw error{};
        return result;
    }

    int num_balls() const
    {
        int result = SDL_JoystickNumBalls(_handle);
        if (result < 0) throw error{};
        return result;
    }

    int num_buttons() const
    {
        int result = SDL_JoystickNumButtons(_handle);
        if (result < 0) throw error{};
        return result;
    }

    int num_hats() const
    {
        int result = SDL_JoystickNumHats(_handle);
        if (result < 0) throw error{};
        return result;
    }

    static bool event_state()
    {
        int result = SDL_JoystickEventState(SDL_QUERY);
        if (result < 0) throw error{};
        return static_cast<bool>(result);
    }

    static void set_event_state(bool enabled) noexcept
    {
        SDL_JoystickEventState(enabled ? SDL_ENABLE : SDL_DISABLE);
    }

    static joystick_ref from(joystick_id id) noexcept;

    static void update() noexcept
    {
        SDL_JoystickUpdate();
    }

    friend bool operator==(const joystick& x, const joystick& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const joystick& x, const joystick& y) noexcept
    {
        return !(x == y);
    }
};

struct joystick_ref : joystick {
    ~joystick_ref()
    {
        _handle = nullptr;
    }

    joystick_ref(const joystick_ref& other) noexcept
        : joystick{other._handle}
    {}

    joystick_ref(const joystick& other) noexcept
        : joystick{other.handle()}
    {}

    explicit joystick_ref(SDL_Joystick* handle) noexcept
        : joystick{handle}
    {}

    SDL_Joystick* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

inline joystick_ref joystick::from(joystick_id id) noexcept
{
    return joystick_ref{SDL_JoystickFromInstanceID(id)};
}

// clang-format on

} // namespace sdl

#endif // SDLW_JOYSTICK_INCLUDED
