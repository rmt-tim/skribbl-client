#ifndef SDLW_HAPTIC_INCLUDED
#define SDLW_HAPTIC_INCLUDED

#include <SDL_haptic.h>

#include <sdlw/joystick.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

struct haptic_device {
    int index;

    const char* name() const
    {
        const char* s = SDL_HapticName(index);
        if (!s) throw error{};
        return s;
    }

    bool opened() noexcept
    {
        return static_cast<bool>(SDL_HapticOpened(index));
    }

    static int count() noexcept
    {
        return SDL_NumHaptics();
    }
};

using haptic_condition = SDL_HapticCondition;
using haptic_constant  = SDL_HapticConstant;
using haptic_custom    = SDL_HapticCustom;
using haptic_direction = SDL_HapticDirection;
using haptic_effect    = SDL_HapticEffect;
using haptic_leftright = SDL_HapticLeftRight;
using haptic_periodic  = SDL_HapticPeriodic;
using haptic_ramp      = SDL_HapticRamp;

class haptic {
    SDL_Haptic* _handle = nullptr;

public:
    ~haptic()
    {
        if (_handle) SDL_HapticClose(_handle);
    }

    haptic() = default;

    haptic(haptic&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    haptic& operator=(haptic&& other) noexcept
    {
        if (_handle) SDL_HapticClose(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit haptic(SDL_Haptic* handle) noexcept
        : _handle{handle}
    {}

    SDL_Haptic* handle() const noexcept
    {
        return _handle;
    }

    enum feature {
        // effects
        constant      = SDL_HAPTIC_CONSTANT,
        sine          = SDL_HAPTIC_SINE,
        leftright     = SDL_HAPTIC_LEFTRIGHT,
        triangle      = SDL_HAPTIC_TRIANGLE,
        sawtooth_up   = SDL_HAPTIC_SAWTOOTHUP,
        sawtooth_down = SDL_HAPTIC_SAWTOOTHDOWN,
        ramp          = SDL_HAPTIC_RAMP,
        spring        = SDL_HAPTIC_SPRING,
        damper        = SDL_HAPTIC_DAMPER,
        inertia       = SDL_HAPTIC_INERTIA,
        friction      = SDL_HAPTIC_FRICTION,
        custom        = SDL_HAPTIC_CUSTOM,
        // features
        gain          = SDL_HAPTIC_GAIN,
        autocenter    = SDL_HAPTIC_AUTOCENTER,
        status        = SDL_HAPTIC_STATUS,
        pause_        = SDL_HAPTIC_PAUSE
    };

    static constexpr unsigned int infinity = SDL_HAPTIC_INFINITY;

    explicit haptic(haptic_device device)
        : _handle{SDL_HapticOpen(device.index)}
    {
        if (!_handle) throw error{};
    }

    explicit haptic(joystick& j)
        : _handle{SDL_HapticOpenFromJoystick(j.handle())}
    {
        if (!_handle) throw error{};
    }

    static haptic from_mouse()
    {
        SDL_Haptic* handle = SDL_HapticOpenFromMouse();
        if (!handle) throw error{};
        return haptic{handle};
    }

    haptic_device device() const
    {
        int index = SDL_HapticIndex(_handle);
        if (index < 0) throw error{};
        return haptic_device{index};
    }

    int num_axes() const
    {
        int result = SDL_HapticNumAxes(_handle);
        if (result < 0) throw error{};
        return result;
    }

    void pause()
    {
        if (SDL_HapticPause(_handle) < 0) throw error{};
    }

    void unpause()
    {
        if (SDL_HapticUnpause(_handle) < 0) throw error{};
    }

    unsigned int features() const
    {
        unsigned int result = SDL_HapticQuery(_handle);
        if (result == 0) throw error{};
        return result;
    }

    void set_autocenter(int autocenter)
    {
        if (SDL_HapticSetAutocenter(_handle, autocenter) < 0) throw error{};
    }

    void set_gain(int gain)
    {
        if (SDL_HapticSetGain(_handle, gain) < 0) throw error{};
    }

    bool rumble_supported() const
    {
        int result = SDL_HapticRumbleSupported(_handle);
        if (result < 0) throw error{};
        return static_cast<bool>(result);
    }

    void rumble_init()
    {
        if (SDL_HapticRumbleInit(_handle) < 0) throw error{};
    }

    void rumble_play(float strength, u32 length)
    {
        if (SDL_HapticRumblePlay(_handle, strength, length) < 0) throw error{};
    }

    void rumble_stop()
    {
        if (SDL_HapticRumbleStop(_handle) < 0) throw error{};
    }

    int num_effects() const
    {
        int result = SDL_HapticNumEffects(_handle);
        if (result < 0) throw error{};
        return result;
    }

    int num_effects_playing() const
    {
        int result = SDL_HapticNumEffectsPlaying(_handle);
        if (result < 0) throw error{};
        return result;
    }

    bool effect_supported(const haptic_effect& e) const
    {
        int result = SDL_HapticEffectSupported(_handle, const_cast<SDL_HapticEffect*>(&e));
        if (result < 0) throw error{};
        return static_cast<bool>(result);
    }

    int create_effect(const haptic_effect& e)
    {
        int effect_id = SDL_HapticNewEffect(_handle, const_cast<SDL_HapticEffect*>(&e));
        if (effect_id < 0) throw error{};
        return effect_id;
    }

    void destroy_effect(int effect_id) noexcept
    {
        SDL_HapticDestroyEffect(_handle, effect_id);
    }

    void update_effect(int effect_id, const haptic_effect& e)
    {
        if (SDL_HapticUpdateEffect(_handle, effect_id, const_cast<SDL_HapticEffect*>(&e)) < 0) {
            throw error{};
        }
    }

    void run_effect(int effect_id, u32 iterations)
    {
        if (SDL_HapticRunEffect(_handle, effect_id, iterations) < 0) {
            throw error{};
        }
    }

    void stop_effect(int effect_id)
    {
        if (SDL_HapticStopEffect(_handle, effect_id) < 0) {
            throw error{};
        }
    }

    void stop_all_effects()
    {
        if (SDL_HapticStopAll(_handle) < 0) throw error{};
    }

    bool is_effect_playing(int effect_id) const
    {
        int result = SDL_HapticGetEffectStatus(_handle, effect_id);
        if (result < 0) throw error{};
        return static_cast<bool>(result);
    }

    friend bool operator==(const haptic& x, const haptic& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const haptic& x, const haptic& y) noexcept
    {
        return !(x == y);
    }
};

inline bool is_haptic(const joystick& j)
{
    int result = SDL_JoystickIsHaptic(j.handle());
    if (result < 0) throw error{};
    return static_cast<bool>(result);
}

inline bool is_mouse_haptic() noexcept
{
    return static_cast<bool>(SDL_MouseIsHaptic());
}

} // namespace sdl

#endif // SDLW_HAPTIC_INCLUDED
