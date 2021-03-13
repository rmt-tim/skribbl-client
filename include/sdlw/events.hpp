#ifndef SDLW_EVENTS_INCLUDED
#define SDLW_EVENTS_INCLUDED

#include <SDL_events.h>

#include <sdlw/audio.hpp>
#include <sdlw/game_controller.hpp>
#include <sdlw/gesture.hpp>
#include <sdlw/joystick.hpp>
#include <sdlw/keyboard.hpp>
#include <sdlw/mouse.hpp>
#include <sdlw/timer.hpp>
#include <sdlw/touch.hpp>
#include <sdlw/video.hpp>

namespace sdl {

enum class event_type : u32 {
    first_event                = SDL_FIRSTEVENT,
    quit                       = SDL_QUIT,
    app_terminating            = SDL_APP_TERMINATING,
    app_low_memory             = SDL_APP_LOWMEMORY,
    app_will_enter_background  = SDL_APP_WILLENTERBACKGROUND,
    app_did_enter_background   = SDL_APP_DIDENTERBACKGROUND,
    app_will_enter_foreground  = SDL_APP_WILLENTERFOREGROUND,
    app_did_enter_foreground   = SDL_APP_DIDENTERFOREGROUND,
    window                     = SDL_WINDOWEVENT,
    syswm                      = SDL_SYSWMEVENT,
    key_down                   = SDL_KEYDOWN,
    key_up                     = SDL_KEYUP,
    text_editing               = SDL_TEXTEDITING,
    text_input                 = SDL_TEXTINPUT,
    keymap_changed             = SDL_KEYMAPCHANGED,
    mouse_motion               = SDL_MOUSEMOTION,
    mouse_button_down          = SDL_MOUSEBUTTONDOWN,
    mouse_button_up            = SDL_MOUSEBUTTONUP,
    mouse_wheel                = SDL_MOUSEWHEEL,
    joy_axis_motion            = SDL_JOYAXISMOTION,
    joy_ball_motion            = SDL_JOYBALLMOTION,
    joy_hat_motion             = SDL_JOYHATMOTION,
    joy_button_down            = SDL_JOYBUTTONDOWN,
    joy_button_up              = SDL_JOYBUTTONUP,
    joy_device_added           = SDL_JOYDEVICEADDED,
    joy_device_removed         = SDL_JOYDEVICEREMOVED,
    controller_axis_motion     = SDL_CONTROLLERAXISMOTION,
    controller_button_down     = SDL_CONTROLLERBUTTONDOWN,
    controller_button_up       = SDL_CONTROLLERBUTTONUP,
    controller_device_added    = SDL_CONTROLLERDEVICEADDED,
    controller_device_removed  = SDL_CONTROLLERDEVICEREMOVED,
    controller_device_remapped = SDL_CONTROLLERDEVICEREMAPPED,
    finger_down                = SDL_FINGERDOWN,
    finger_up                  = SDL_FINGERUP,
    finger_motion              = SDL_FINGERMOTION,
    dollar_gesture             = SDL_DOLLARGESTURE,
    dollar_record              = SDL_DOLLARRECORD,
    multi_gesture              = SDL_MULTIGESTURE,
    clipboard_update           = SDL_CLIPBOARDUPDATE,
    drop_file                  = SDL_DROPFILE,
    drop_text                  = SDL_DROPTEXT,
    drop_begin                 = SDL_DROPBEGIN,
    drop_complete              = SDL_DROPCOMPLETE,
    audio_device_added         = SDL_AUDIODEVICEADDED,
    audio_device_removed       = SDL_AUDIODEVICEREMOVED,
    render_targets_reset       = SDL_RENDER_TARGETS_RESET,
    render_device_reset        = SDL_RENDER_DEVICE_RESET,
    user_event                 = SDL_USEREVENT,
    last_event                 = SDL_LASTEVENT
};

struct common_event {
    event_type type;
    sdl::clock::time_point timestamp;
};

struct display_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 display;
    u8 event; // TODO: SDL_DisplayEventID
    u8 _padding1;
    u8 _padding2;
    u8 _padding3;
    i32 data1;
};

struct window_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 window_id;
    u8 event; // TODO: SDL_WindowEventID
    u8 _padding1;
    u8 _padding2;
    u8 _padding3;
    i32 data1;
    i32 data2;
};

struct keyboard_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 window_id;
    u8 state; // pressed/released
    u8 repeat;
    u8 _padding2;
    u8 _padding3;
    key key;
};

struct text_editing_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 window_id;
    char text[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    i32 start;
    i32 length;
};

struct text_input_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 window_id;
    char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
};

struct mouse_motion_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 window_id;
    u32 which; // mouse instance id
    u32 state;
    i32 x;
    i32 y;
    i32 xrel;
    i32 yrel;
};

struct mouse_button_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 window_id;
    u32 which;
    u8 button;
    u8 state;
    u8 clicks;
    u8 _padding1;
    i32 x;
    i32 y;
};

struct mouse_wheel_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 window_id;
    u32 which;
    i32 x;
    i32 y;
    u32 direction;
};

struct joy_axis_event {
    event_type type;
    sdl::clock::time_point timestamp;
    joystick_id which;
    u8 axis;
    u8 _padding1;
    u8 _padding2;
    u8 _padding3;
    i16 value;
    u16 _padding4;
};

struct joy_ball_event {
    event_type type;
    sdl::clock::time_point timestamp;
    joystick_id which;
    u8 ball;
    u8 _padding1;
    u8 _padding2;
    u8 _padding3;
    i16 xrel;
    i16 yrel;
};

struct joy_hat_event {
    event_type type;
    sdl::clock::time_point timestamp;
    joystick_id which;
    u8 hat;
    u8 value;
    u8 _padding1;
    u8 _padding2;
};

struct joy_button_event {
    event_type type;
    sdl::clock::time_point timestamp;
    joystick_id which;
    u8 button;
    u8 state;
    u8 _padding1;
    u8 _padding2;
};

struct joy_device_event {
    event_type type;
    sdl::clock::time_point timestamp;
    i32 which;
};

struct controller_axis_event {
    event_type type;
    sdl::clock::time_point timestamp;
    joystick_id which;
    u8 axis;
    u8 _padding1;
    u8 _padding2;
    u8 _padding3;
    i16 value;
    i16 _padding4;
};

struct controller_button_event {
    event_type type;
    sdl::clock::time_point timestamp;
    joystick_id which;
    u8 button;
    u8 state;
    u8 _padding1;
    u8 _padding2;
};

struct controller_device_event {
    event_type type;
    sdl::clock::time_point timestamp;
    i32 which;
};

struct audio_device_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 which;
    u8 iscapture;
    u8 _padding1;
    u8 _padding2;
    u8 _padding3;
};

struct touch_finger_event {
    event_type type;
    sdl::clock::time_point timestamp;
    touch_id touchId;
    finger_id fingerId;
    float x;
    float y;
    float dx;
    float dy;
    float pressure;
};

struct multi_gesture_event {
    event_type type;
    sdl::clock::time_point timestamp;
    touch_id touch_id;
    float dtheta;
    float ddist;
    float x;
    float y;
    u16 num_fingers;
    u16 padding;
};

struct dollar_gesture_event {
    event_type type;
    sdl::clock::time_point timestamp;
    touch_id touch_id;
    gesture_id gesture_id;
    u32 num_fingers;
    float error;
    float x;
    float y;
};

struct drop_event {
    event_type type;
    sdl::clock::time_point timestamp;
    char* file;
    u32 window_id;
};

struct sensor_event {
    event_type type;
    sdl::clock::time_point timestamp;
    i32 which;
    float data[6];
};

struct quit_event {
    event_type type;
    sdl::clock::time_point timestamp;
};

struct os_event {
    event_type type;
    sdl::clock::time_point timestamp;
};

struct user_event {
    event_type type;
    sdl::clock::time_point timestamp;
    u32 window_id;
    i32 code;
    void* data1;
    void* data2;
};

struct syswm_event {
    Uint32 type;
    Uint32 timestamp;
    SDL_SysWMmsg* msg;
};

union event {
    event_type type;
    common_event common;
    display_event display;
    window_event window;
    keyboard_event key;
    text_editing_event edit;
    text_input_event text;
    mouse_motion_event motion;
    mouse_button_event button;
    mouse_wheel_event wheel;
    joy_axis_event jaxis;
    joy_ball_event jball;
    joy_hat_event jhat;
    joy_button_event jbutton;
    joy_device_event jdevice;
    controller_axis_event caxis;
    controller_button_event cbutton;
    controller_device_event cdevice;
    audio_device_event adevice;
    sensor_event sensor;
    quit_event quit;
    user_event user;
    syswm_event syswm;
    touch_finger_event tfinger;
    multi_gesture_event mgesture;
    dollar_gesture_event dgesture;
    drop_event drop;
    u8 _padding[56];

    event() {}
};

enum class window_event_type : u8 {
    none         = SDL_WINDOWEVENT_NONE,
    shown        = SDL_WINDOWEVENT_SHOWN,
    hidden       = SDL_WINDOWEVENT_HIDDEN,
    exposed      = SDL_WINDOWEVENT_EXPOSED,
    moved        = SDL_WINDOWEVENT_MOVED,
    resized      = SDL_WINDOWEVENT_RESIZED,
    size_changed = SDL_WINDOWEVENT_SIZE_CHANGED,
    minimized    = SDL_WINDOWEVENT_MINIMIZED,
    maximized    = SDL_WINDOWEVENT_MAXIMIZED,
    restored     = SDL_WINDOWEVENT_RESTORED,
    enter        = SDL_WINDOWEVENT_ENTER,
    leave        = SDL_WINDOWEVENT_LEAVE,
    focus_gained = SDL_WINDOWEVENT_FOCUS_GAINED,
    focus_lost   = SDL_WINDOWEVENT_FOCUS_LOST,
    close        = SDL_WINDOWEVENT_CLOSE,
    take_focus   = SDL_WINDOWEVENT_TAKE_FOCUS,
    hit_test     = SDL_WINDOWEVENT_HIT_TEST
};

inline void pump_events() noexcept
{
    SDL_PumpEvents();
}

inline bool event_state(event_type etype) noexcept
{
    return SDL_GetEventState(static_cast<u32>(etype)) == SDL_ENABLE ? false : true;
}

inline void set_event_state(event_type etype, bool enable) noexcept
{
    SDL_EventState(static_cast<u32>(etype), static_cast<SDL_bool>(!enable));
}

class event_filter {
    SDL_EventFilter _filter = nullptr;
    void* _userdata = nullptr;

public:
    using function_type = bool(const event&);

    event_filter() = default;

    explicit event_filter(function_type* f) noexcept
        : _userdata{reinterpret_cast<void*>(f)}
    {
        _filter = [](void* userdata, SDL_Event* e) -> int {
            const auto func = reinterpret_cast<function_type*>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            return static_cast<int>(func(ev));
        };
    }

    template<typename EventFilter>
    explicit event_filter(EventFilter& f) noexcept
        : _userdata{&f}
    {
        static_assert(std::is_invocable_r_v<bool, EventFilter, const event&>);
        _filter = [](void* userdata, SDL_Event* e) -> int {
            auto& func = *static_cast<EventFilter*>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            return static_cast<int>(func(ev));
        };
    }

    event_filter(SDL_EventFilter f, void* userdata) noexcept
        : _filter{f}
        , _userdata{userdata}
    {}

    SDL_EventFilter filter() const noexcept
    {
        return _filter;
    }

    void* userdata() const noexcept
    {
        return _userdata;
    }

    bool operator()(const event& e) const noexcept
    {
        const auto ev = const_cast<SDL_Event*>(reinterpret_cast<const SDL_Event*>(&e));
        return static_cast<bool>(_filter(_userdata, ev));
    }

    friend bool operator==(const event_filter& x, const event_filter& y) noexcept
    {
        return x.filter() == y.filter() && x.userdata() == y.userdata();
    }

    friend bool operator!=(const event_filter& x, const event_filter& y) noexcept
    {
        return !(x == y);
    }

    static void set(event_filter f) noexcept
    {
        SDL_SetEventFilter(f._filter, f._userdata);
    }

    static void set(function_type* f) noexcept
    {
        set(event_filter{f});
    }

    template<typename Callable>
    static void set(Callable& c) noexcept
    {
        set(event_filter{c});
    }

    static optional<event_filter> get() noexcept
    {
        SDL_EventFilter filter;
        void* userdata;
        if (SDL_GetEventFilter(&filter, &userdata)) {
            return event_filter{filter, userdata};
        } else {
            return {};
        }
    }
};

namespace detail {

template<typename EventWatch>
struct watch_generator {
    static int sdl_callback(void* userdata, SDL_Event* e)
    {
        auto& watch = *static_cast<EventWatch*>(userdata);
        const auto& ev = *reinterpret_cast<event*>(e);
        watch(ev);
        return 0; // return value is ignored by SDL2
    }
};

inline int fp_sdl_callback(void* userdata, SDL_Event* e)
{
    auto watch = reinterpret_cast<void (*)(const event&)>(userdata);
    const auto& ev = *reinterpret_cast<event*>(e);
    watch(ev);
    return 0; // return value is ignored by SDL2
}

} // namespace detail

template<typename EventWatch>
void add_event_watch(EventWatch& w)
{
    static_assert(std::is_invocable_r_v<void, EventWatch, const event&>);
    SDL_AddEventWatch(detail::watch_generator<EventWatch>::sdl_callback, &w);
}

inline void add_event_watch(void (*watch)(const event&))
{
    SDL_AddEventWatch(detail::fp_sdl_callback, reinterpret_cast<void*>(watch));
}

template<typename EventWatch>
void remove_event_watch(EventWatch& w)
{
    static_assert(std::is_invocable_r_v<void, EventWatch, const event&>);
    SDL_DelEventWatch(detail::watch_generator<EventWatch>::sdl_callback, &w);
}

inline void remove_event_watch(void (*watch)(const event&))
{
    SDL_DelEventWatch(detail::fp_sdl_callback, reinterpret_cast<void*>(watch));
}

namespace event_queue {

inline void flush(event_type etype) noexcept
{
    SDL_FlushEvent(static_cast<u32>(etype));
}

inline void flush(event_type min, event_type max)
{
    SDL_FlushEvents(static_cast<u32>(min), static_cast<u32>(max));
}

template<typename EventFilter>
void filter(EventFilter& f)
{
    static_assert(std::is_invocable_r_v<bool, EventFilter, const event&>);
    constexpr auto sdl_callback = [](void* userdata, SDL_Event* e) -> int {
        auto& filter = *static_cast<EventFilter*>(userdata);
        const auto& ev = *reinterpret_cast<event*>(e);
        return static_cast<bool>(filter(ev));
    };
    SDL_FilterEvents(sdl_callback, &f);
}

inline void filter(bool (*filter)(const event&))
{
    auto fp_sdl_callback = [](void* userdata, SDL_Event* e) -> int {
        auto f = reinterpret_cast<bool (*)(const event&)>(userdata);
        const auto& ev = *reinterpret_cast<event*>(e);
        return static_cast<bool>(f(ev));
    };
    SDL_FilterEvents(fp_sdl_callback, reinterpret_cast<void*>(filter));
}

inline bool has(event_type etype) noexcept
{
    return SDL_HasEvent(static_cast<u32>(etype));
}

inline bool has(event_type min, event_type max) noexcept
{
    return SDL_HasEvents(static_cast<u32>(min), static_cast<u32>(max));
}

inline bool was_quit_requested() noexcept
{
    return SDL_QuitRequested();
}

inline bool push(const event& e)
{
    const auto result = SDL_PushEvent(reinterpret_cast<SDL_Event*>(const_cast<event*>(&e)));
    if (result < 0) {
        throw error{};
    } else {
        return static_cast<bool>(result);
    }
}

inline bool poll(event& e) noexcept
{
    return SDL_PollEvent(reinterpret_cast<SDL_Event*>(&e));
}

inline int add(span<const event, int> events, event_type min, event_type max)
{
    const auto cevents = const_cast<event*>(events.data());
    const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
    const auto size = static_cast<int>(events.size());
    const auto amin = static_cast<u32>(min);
    const auto amax = static_cast<u32>(max);
    const auto res = SDL_PeepEvents(p_events, size, SDL_ADDEVENT, amin, amax);
    if (res < 0) {
        throw error{};
    } else {
        return res;
    }
}

inline int peek(span<event, int> events, event_type min, event_type max)
{
    const auto cevents = const_cast<event*>(events.data());
    const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
    const auto size = static_cast<int>(events.size());
    const auto amin = static_cast<u32>(min);
    const auto amax = static_cast<u32>(max);
    const auto res = SDL_PeepEvents(p_events, size, SDL_PEEKEVENT, amin, amax);
    if (res < 0) {
        throw error{};
    } else {
        return res;
    }
}

inline int get(span<event, int> events, event_type min, event_type max)
{
    const auto cevents = const_cast<event*>(events.data());
    const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
    const auto size = static_cast<int>(events.size());
    const auto amin = static_cast<u32>(min);
    const auto amax = static_cast<u32>(max);
    const auto res = SDL_PeepEvents(p_events, size, SDL_GETEVENT, amin, amax);
    if (res < 0) {
        throw error{};
    } else {
        return res;
    }
}

inline bool await(event& e) noexcept
{
    return SDL_WaitEvent(reinterpret_cast<SDL_Event*>(&e)) == 1;
}

inline bool await(event& e, clock::duration timeout) noexcept
{
    return SDL_WaitEventTimeout(reinterpret_cast<SDL_Event*>(&e), timeout.count());
}

} // namespace event_queue

} // namespace sdl

#endif // SDLW_EVENTS_INCLUDED
