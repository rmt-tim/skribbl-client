#ifndef SDLW_MUTEX_INCLUDED
#define SDLW_MUTEX_INCLUDED

#include <SDL_mutex.h>

#include <sdlw/timer.hpp>

namespace sdl {

class mutex {
    SDL_mutex* _handle = nullptr;

public:
    ~mutex()
    {
        if (_handle) SDL_DestroyMutex(_handle);
    }

    mutex()
        : _handle{SDL_CreateMutex()}
    {
        if (!_handle) throw error{};
    }

    mutex(mutex&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    mutex& operator=(mutex&& other) noexcept
    {
        if (_handle) SDL_DestroyMutex(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit mutex(SDL_mutex* handle) noexcept
        : _handle{handle}
    {}

    SDL_mutex* handle() const noexcept
    {
        return _handle;
    }

    void lock()
    {
        if (SDL_LockMutex(_handle) < 0) throw error{};
    }

    void unlock()
    {
        if (SDL_UnlockMutex(_handle) < 0) throw error{};
    }

    bool try_lock()
    {
        int result = SDL_TryLockMutex(_handle);
        if (result == -1) throw error{};
        return result == 0;
    }

    friend bool operator==(const mutex& x, const mutex& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const mutex& x, const mutex& y) noexcept
    {
        return !(x == y);
    }
};

class semaphore {
    SDL_sem* _handle = nullptr;

public:
    ~semaphore()
    {
        if (_handle) SDL_DestroySemaphore(_handle);
    }

    semaphore() = default;

    semaphore(semaphore&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    semaphore& operator=(semaphore&& other) noexcept
    {
        if (_handle) SDL_DestroySemaphore(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit semaphore(SDL_sem* handle) noexcept
        : _handle{handle}
    {}

    SDL_sem* handle() const noexcept
    {
        return _handle;
    }

    explicit semaphore(u32 initial_value)
        : _handle{SDL_CreateSemaphore(initial_value)}
    {
        if (!_handle) throw error{};
    }

    void post()
    {
        if (SDL_SemPost(_handle) < 0) throw error{};
    }

    u32 value()
    {
        return SDL_SemValue(_handle);
    }

    bool try_wait()
    {
        int result = SDL_SemTryWait(_handle);
        if (result < 0) throw error{};
        return result == 0;
    }

    void wait()
    {
        if (SDL_SemWait(_handle) < 0) throw error{};
    }

    bool wait_for(sdl::clock::duration timeout)
    {
        int result = SDL_SemWaitTimeout(_handle, timeout.count());
        if (result < 0) throw error{};
        return result == 0;
    }

    friend bool operator==(const semaphore& x, const semaphore& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const semaphore& x, const semaphore& y) noexcept
    {
        return !(x == y);
    }
};

class condition_variable {
    SDL_cond* _handle = nullptr;

public:
    ~condition_variable()
    {
        if (_handle) SDL_DestroyCond(_handle);
    }

    condition_variable()
        : _handle{SDL_CreateCond()}
    {
        if (!_handle) throw error{};
    }

    condition_variable(condition_variable&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    condition_variable& operator=(condition_variable&& other) noexcept
    {
        if (_handle) SDL_DestroyCond(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit condition_variable(SDL_cond* handle) noexcept
        : _handle{handle}
    {}

    SDL_cond* handle() const noexcept
    {
        return _handle;
    }

    void broadcast()
    {
        if (SDL_CondBroadcast(_handle) < 0) throw error{};
    }

    void signal()
    {
        if (SDL_CondSignal(_handle) < 0) throw error{};
    }

    void wait(mutex& m)
    {
        if (SDL_CondWait(_handle, m.handle()) < 0) throw error{};
    }

    bool wait_for(mutex& m, sdl::clock::duration timeout)
    {
        int result = SDL_CondWaitTimeout(_handle, m.handle(), timeout.count());
        if (result < 0) throw error{};
        return result == 0;
    }

    friend bool operator==(const condition_variable& x, const condition_variable& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const condition_variable& x, const condition_variable& y) noexcept
    {
        return !(x == y);
    }
};

} // namespace sdl

#endif // SDLW_MUTEX_INCLUDED
