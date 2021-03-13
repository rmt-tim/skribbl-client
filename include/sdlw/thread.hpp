#ifndef SDLW_THREAD_INCLUDED
#define SDLW_THREAD_INCLUDED

#include <SDL_thread.h>

namespace sdl {

enum class thread_id : SDL_threadID {};

enum class thread_priority {
    low    = SDL_THREAD_PRIORITY_LOW,
    normal = SDL_THREAD_PRIORITY_NORMAL,
    high   = SDL_THREAD_PRIORITY_HIGH
};

class thread {
    SDL_Thread* _handle = nullptr;

public:
    ~thread()
    {
        wait();
    }

    thread() = default;

    template <typename Function>
    thread(const char* name, Function&& f)
    {
        using function_type = typename std::decay<Function>::type;
        function_type* function = new function_type(static_cast<Function&&>(f));
        auto thread_function = [] (void* data) -> int {
            function_type* fn = static_cast<function_type*>(data);
            int status = (*fn)();
            delete fn;
            return status;
        };
        _handle = SDL_CreateThread(thread_function, name);
        if (!_handle) throw error{};
    }

    thread_id id() const noexcept
    {
        return static_cast<thread_id>(SDL_GetThreadID(_handle));
    }

    const char* name() const noexcept
    {
        return SDL_GetThreadName(_handle);
    }

    void set_priority(thread_priority p) noexcept
    {
        SDL_SetThreadPriority(static_cast<SDL_ThreadPriority>(p));
    }

    void detach() noexcept
    {
        SDL_DetachThread(_handle);
    }

    int wait() noexcept
    {
        int status;
        SDL_WaitThread(_handle, &status);
        return status;
    }
};

struct this_thread {
    static thread_id id() { return SDL_ThreadID(); }
};

} // namespace sdl

#endif // SDLW_THREAD_INCLUDED
