#ifndef SDLW_LOADSO_INCLUDED
#define SDLW_LOADSO_INCLUDED

#include <SDL_loadso.h>

#include <sdlw/error.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

class shared_object {
    void* _handle = nullptr;

public:
    ~shared_object()
    {
        if (_handle) SDL_UnloadObject(_handle);
    }

    shared_object() = default;

    shared_object(shared_object&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    shared_object& operator=(shared_object&& other) noexcept
    {
        if (_handle) SDL_UnloadObject(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit shared_object(void* handle) noexcept
        : _handle{handle}
    {}

    void* handle() const noexcept
    {
        return _handle;
    }

    explicit shared_object(const char* sofile)
        : _handle{SDL_LoadObject(sofile)}
    {
        if (!_handle) throw error{};
    }

    void* load_function(const char* name) const
    {
        void* f = SDL_LoadFunction(_handle, name);
        if (!f) throw error{};
        return f;
    }

    friend bool operator==(const shared_object& x, const shared_object& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const shared_object& x, const shared_object& y) noexcept
    {
        return !(x == y);
    }
};

} // namespace sdl

#endif // SDLW_LOADSO_INCLUDED
