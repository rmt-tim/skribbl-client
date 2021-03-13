#ifndef SDLW_GAME_CONTROLLER_INCLUDED
#define SDLW_GAME_CONTROLLER_INCLUDED

#include <SDL_gamecontroller.h>

#include <sdlw/joystick.hpp>
#include <sdlw/rwops.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

// clang-format off

enum class game_controller_axis {
    invalid       = SDL_CONTROLLER_AXIS_INVALID,
    left_x        = SDL_CONTROLLER_AXIS_LEFTX,
    left_y        = SDL_CONTROLLER_AXIS_LEFTY,
    right_x       = SDL_CONTROLLER_AXIS_RIGHTX,
    right_y       = SDL_CONTROLLER_AXIS_RIGHTY,
    trigger_left  = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    max           = SDL_CONTROLLER_AXIS_MAX
};

inline game_controller_axis to_game_controller_axis(const char* pch_string) noexcept
{
    return static_cast<game_controller_axis>(SDL_GameControllerGetAxisFromString(pch_string));
}

inline const char* to_string(game_controller_axis axis) noexcept
{
    return SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(axis));
}

enum class game_controller_button {
    invalid        = SDL_CONTROLLER_BUTTON_INVALID,
    a              = SDL_CONTROLLER_BUTTON_A,
    b              = SDL_CONTROLLER_BUTTON_B,
    x              = SDL_CONTROLLER_BUTTON_X,
    y              = SDL_CONTROLLER_BUTTON_Y,
    back           = SDL_CONTROLLER_BUTTON_BACK,
    guide          = SDL_CONTROLLER_BUTTON_GUIDE,
    start          = SDL_CONTROLLER_BUTTON_START,
    left_stick     = SDL_CONTROLLER_BUTTON_LEFTSTICK,
    right_stick    = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
    left_shoulder  = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    right_shoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    dpad_up        = SDL_CONTROLLER_BUTTON_DPAD_UP,
    dpad_down      = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    dpad_left      = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
    dpad_right     = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    max            = SDL_CONTROLLER_BUTTON_MAX
};

inline game_controller_button to_game_controller_button(const char* pch_string) noexcept
{
    return static_cast<game_controller_button>(SDL_GameControllerGetButtonFromString(pch_string));
}

inline const char* to_string(game_controller_button button) noexcept
{
    return SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(button));
}

enum class game_controller_bind_type : std::underlying_type_t<SDL_GameControllerBindType> {
    none   = SDL_CONTROLLER_BINDTYPE_NONE,
    button = SDL_CONTROLLER_BINDTYPE_BUTTON,
    axis   = SDL_CONTROLLER_BINDTYPE_AXIS,
    hat    = SDL_CONTROLLER_BINDTYPE_HAT
};

class game_controller_button_bind {
public:
    explicit game_controller_button_bind(SDL_GameControllerButtonBind bind) noexcept
        : _bind{bind}
    {}

    game_controller_bind_type type() const noexcept
    {
        return static_cast<game_controller_bind_type>(_bind.bindType);
    }

    int button() const noexcept
    {
        return _bind.value.button;
    }

    int axis() const noexcept
    {
        return _bind.value.axis;
    }

    int hat() const noexcept
    {
        return _bind.value.hat.hat;
    }

    int hat_mask() const noexcept
    {
        return _bind.value.hat.hat_mask;
    }

private:
    SDL_GameControllerButtonBind _bind = {};
};

struct game_controller_ref;

class game_controller {
protected:
    SDL_GameController* _handle = nullptr;

public:
    ~game_controller()
    {
        if (_handle) SDL_GameControllerClose(_handle);
    }

    game_controller() = default;

    game_controller(game_controller&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    game_controller& operator=(game_controller&& other) noexcept
    {
        if (_handle) SDL_GameControllerClose(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit game_controller(SDL_GameController* handle) noexcept
        : _handle{handle}
    {}

    SDL_GameController* handle() const noexcept
    {
        return _handle;
    }

    game_controller(game_controller_ref) = delete;

    explicit game_controller(int joystick_index)
        : _handle{SDL_GameControllerOpen(joystick_index)}
    {
        if (!_handle) throw error{};
    }

    bool attached() const noexcept
    {
        return static_cast<bool>(SDL_GameControllerGetAttached(_handle));
    }

    i16 axis(game_controller_axis axis) const noexcept
    {
        return SDL_GameControllerGetAxis(_handle, static_cast<SDL_GameControllerAxis>(axis));
    }

    game_controller_button_bind bind(game_controller_axis axis) const noexcept
    {
        const auto axis_ = static_cast<SDL_GameControllerAxis>(axis);
        const auto result = SDL_GameControllerGetBindForAxis(_handle, axis_);
        return static_cast<game_controller_button_bind>(result);
    }

    game_controller_button_bind bind(game_controller_button button) const noexcept
    {
        const auto button_ = static_cast<SDL_GameControllerButton>(button);
        const auto result = SDL_GameControllerGetBindForButton(_handle, button_);
        return static_cast<game_controller_button_bind>(result);
    }

    bool button(game_controller_button button) const noexcept
    {
        const auto button_ = static_cast<SDL_GameControllerButton>(button);
        const auto result = SDL_GameControllerGetButton(_handle, button_);
        return static_cast<bool>(result);
    }

    joystick_ref joystick() noexcept
    {
        return joystick_ref{SDL_GameControllerGetJoystick(_handle)};
    }

    string mapping() const noexcept
    {
        return string{SDL_GameControllerMapping(_handle)};
    }

    const char* name() const noexcept
    {
        return SDL_GameControllerName(_handle);
    }

    static bool add_mapping(const char* mapping_string)
    {
        const auto result = SDL_GameControllerAddMapping(mapping_string);
        if (result < 0) throw error{};
        return static_cast<bool>(result);
    }

    static int add_mappings(const char* filename)
    {
        const auto result = SDL_GameControllerAddMappingsFromFile(filename);
        if (result < 0) throw error{};
        return result;
    }

    static int add_mappings(stream& s)
    {
        const auto result = SDL_GameControllerAddMappingsFromRW(s.get_pointer(), 0);
        if (result < 0) throw error{};
        return result;
    }

    static bool event_state() noexcept
    {
        return static_cast<bool>(SDL_GameControllerEventState(SDL_QUERY));
    }

    static void set_event_state(bool enabled) noexcept
    {
        SDL_GameControllerEventState(enabled ? SDL_ENABLE : SDL_DISABLE);
    }

    static game_controller_ref from(joystick_id joyid) noexcept;

    static string mapping(joystick_guid guid) noexcept
    {
        SDL_JoystickGUID guid_;
        std::memcpy(&guid_, guid.data, 16);
        const auto str = SDL_GameControllerMappingForGUID(guid_);
        return string{str};
    }

    static const char* name(int joystick_index) noexcept
    {
        return SDL_GameControllerNameForIndex(joystick_index);
    }

    static void update() noexcept
    {
        SDL_GameControllerUpdate();
    }

    friend bool operator==(const game_controller& x, const game_controller& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const game_controller& x, const game_controller& y) noexcept
    {
        return !(x == y);
    }
};

struct game_controller_ref : game_controller {
    ~game_controller_ref()
    {
        _handle = nullptr;
    }

    game_controller_ref(const game_controller_ref& other) noexcept
        : game_controller{other._handle}
    {}

    game_controller_ref(const game_controller& other) noexcept
        : game_controller{other.handle()}
    {}

    explicit game_controller_ref(SDL_GameController* handle) noexcept
        : game_controller{handle}
    {}

    SDL_GameController* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

inline game_controller_ref game_controller::from(joystick_id joyid) noexcept
{
    return game_controller_ref{SDL_GameControllerFromInstanceID(joyid)};
}

inline bool is_game_controller(int joystick_index) noexcept
{
    return static_cast<bool>(SDL_IsGameController(joystick_index));
}

// clang-format on

} // namespace sdl

#endif // SDLW_GAME_CONTROLLER_INCLUDED
