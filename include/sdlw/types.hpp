#ifndef SDLW_TYPES_INCLUDED
#define SDLW_TYPES_INCLUDED

#include <SDL_types.h>

/* #include "sdlw/detail/span.hpp" */

namespace sdl {

/* using namespace nonstd; */

using i8  = Sint8;
using u8  = Uint8;
using i16 = Sint16;
using u16 = Uint16;
using i32 = Sint32;
using u32 = Uint32;
using i64 = Sint64;
using u64 = Uint64;

template <class T, class Size = int>
class span {
    T* _data = nullptr;
    Size _size = 0;

public:
    span() = default;

    span(T* data, Size size) noexcept
        : _data{data}
        , _size{size}
    {}

    template <std::size_t N>
    span(T (&array)[N]) noexcept
        : _data{array}
        , _size{N}
    {}

    T* data() const noexcept
    {
        return _data;
    }

    Size size() const noexcept
    {
        return _size;
    }

    T& operator[](Size index) const noexcept
    {
        return _data[index];
    }
};

template <class T, class Size, class Deleter>
class array {
    T* _data = nullptr;
    Size _size = 0;
    Deleter _deleter;

public:
    ~array()
    {
        if (_data != nullptr) {
            _deleter(_data);
        }
    }

    array() = default;

    array(const array&) = delete;

    array& operator=(const array&) = delete;

    array(array&& other) noexcept
        : _data{other._data}
        , _size{other._size}
        , _deleter{static_cast<Deleter&&>(other._deleter)}
    {
        other._data = nullptr;
        other._size = 0;
    }

    array& operator=(array&& other) noexcept
    {
        this->~array();
        _data = other._data;
        _size = other._size;
        _deleter = static_cast<Deleter&&>(other._deleter);
        other._data = nullptr;
        other._size = nullptr;
        return *this;
    }

    array(T* data, Size size) noexcept
        : _data{data}
        , _size{size}
    {}

    T* data() noexcept
    {
        return _data;
    }

    const T* data() const noexcept
    {
        return _data;
    }

    Size size() const noexcept
    {
        return _size;
    }

    T& operator[](Size index) noexcept
    {
        return _data[index];
    }

    const T& operator[](Size index) const noexcept
    {
        return _data[index];
    }
};

class string {
    char* _data = nullptr;

public:
    ~string()
    {
        if (_data != nullptr) {
            SDL_free(_data);
        }
    }

    string() = default;

    explicit string(char* cstring) noexcept
        : _data{cstring}
    {}

    char* data() noexcept
    {
        return _data;
    }

    const char* data() const noexcept
    {
        return _data;
    }
};

template <class T>
class optional {
    T _value;
    bool _present = false;

public:
    optional() = default;

    constexpr optional(const T& value) noexcept
        : _value{value}
        , _present{true}
    {}

    constexpr explicit operator bool() const noexcept
    {
        return _present;
    }

    T& operator*() noexcept
    {
        return _value;
    }

    const T& operator*() const noexcept
    {
        return _value;
    }
};

} // namespace sdl

#endif // SDLW_TYPES_INCLUDED
