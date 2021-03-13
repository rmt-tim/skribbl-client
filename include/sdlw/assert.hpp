#ifndef SDLW_ASSERT_INCLUDED
#define SDLW_ASSERT_INCLUDED

#include <type_traits>

#include <SDL_assert.h>

// clang-format off

#define SDL_ASSERT          SDL_assert
#define SDL_ASSERT_RELEASE  SDL_assert_release
#define SDL_ASSERT_PARANOID SDL_assert_paranoid

namespace sdl {

enum class assert_state : std::underlying_type_t<SDL_AssertState> {
    retry         = SDL_ASSERTION_RETRY,
    break_        = SDL_ASSERTION_BREAK,
    abort         = SDL_ASSERTION_ABORT,
    ignore        = SDL_ASSERTION_IGNORE,
    always_ignore = SDL_ASSERTION_ALWAYS_IGNORE
};

class assert_data_ref {
public:
    explicit operator bool() const noexcept { return static_cast<bool>(_assert_data); }
    explicit assert_data_ref(const SDL_AssertData* ad) noexcept : _assert_data{ad} {}
    const SDL_AssertData* get_pointer() const noexcept { return _assert_data; }

    bool            always_ignore() const noexcept { return static_cast<bool>(_assert_data->always_ignore); }
    unsigned        trigger_count() const noexcept { return _assert_data->trigger_count;                    }
    const char*     condition()     const noexcept { return _assert_data->condition;                        }
    const char*     filename()      const noexcept { return _assert_data->filename;                         }
    int             line_number()   const noexcept { return _assert_data->linenum;                          }
    const char*     function()      const noexcept { return _assert_data->function;                         }
    assert_data_ref next()          const noexcept { return assert_data_ref{_assert_data->next};            }

private:
    const SDL_AssertData* _assert_data = nullptr;
};

// clang-format on

class assertion_handler {
public:
    using function_type = assert_state(assert_data_ref);

    explicit assertion_handler(function_type* f) noexcept
        : _userdata{reinterpret_cast<void*>(f)}
    {
        _function = [](const SDL_AssertData* data, void* userdata) -> SDL_AssertState {
            const auto func = reinterpret_cast<function_type*>(userdata);
            const auto result = func(assert_data_ref{data});
            return static_cast<SDL_AssertState>(result);
        };
    }

    template<typename Callable>
    explicit assertion_handler(Callable& c) noexcept
        : _userdata{&c}
    {
        static_assert(std::is_invocable_r_v<assert_state, Callable, assert_data_ref>);
        _function = [](const SDL_AssertData* data, void* userdata) -> SDL_AssertState {
            auto& func = *static_cast<Callable*>(userdata);
            const auto result = func(assert_data_ref{data});
            return static_cast<SDL_AssertState>(result);
        };
    }

    assertion_handler(SDL_AssertionHandler assertion_handler, void* userdata) noexcept
        : _function{assertion_handler}
        , _userdata{userdata}
    {}

    SDL_AssertionHandler function() const noexcept
    {
        return _function;
    }

    void* userdata() const noexcept
    {
        return _userdata;
    }

    assert_state operator()(assert_data_ref ad) const noexcept
    {
        return static_cast<assert_state>(_function(ad.get_pointer(), _userdata));
    }

    friend bool operator==(const assertion_handler& lhs, const assertion_handler& rhs) noexcept
    {
        return lhs.function() == rhs.function() && lhs.userdata() == rhs.userdata();
    }

    friend bool operator!=(const assertion_handler& lhs, const assertion_handler& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    static void set(assertion_handler handler) noexcept
    {
        SDL_SetAssertionHandler(handler.function(), handler.userdata());
    }

    static void set(function_type* f) noexcept
    {
        set(assertion_handler{f});
    }

    template<typename Callable>
    static void set(Callable& c) noexcept
    {
        set(assertion_handler{c});
    }

    static assertion_handler get() noexcept
    {
        auto userdata = static_cast<void*>(nullptr);
        auto function = SDL_GetAssertionHandler(&userdata);
        return {function, userdata};
    }

    static assertion_handler get_default() noexcept
    {
        return {SDL_GetDefaultAssertionHandler(), nullptr};
    }

private:
    SDL_AssertionHandler _function = nullptr;
    void* _userdata = nullptr;
};

inline assert_data_ref get_assertion_report() noexcept
{
    return assert_data_ref{SDL_GetAssertionReport()};
}

inline void reset_assertion_report() noexcept
{
    SDL_ResetAssertionReport();
}

inline void trigger_breakpoint() noexcept
{
    SDL_TriggerBreakpoint();
}

} // namespace sdl

#endif // SDLW_ASSERT_INCLUDED
