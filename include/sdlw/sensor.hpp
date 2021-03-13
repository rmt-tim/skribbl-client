#ifndef SDLW_SENSOR_INCLUDED
#define SDLW_SENSOR_INCLUDED

#include <SDL_sensor.h>

namespace sdl {

using sensor_id = SDL_SensorID;

enum class sensor_type {
    invalid       = SDL_SENSOR_INVALID,
    unknown       = SDL_SENSOR_UNKNOWN,
    accelerometer = SDL_SENSOR_ACCEL,
    gyroscope     = SDL_SENSOR_GYRO
};

struct sensor_ref;

struct sensor_device {
    int index;

    static int count() noexcept
    {
        return SDL_NumSensors();
    }

    sensor_id id() const
    {
        sensor_id id = SDL_SensorGetDeviceInstanceID(index);
        if (id == -1) throw error{};
        return id;
    }

    const char* name() const
    {
        const char* s = SDL_SensorGetDeviceName(index);
        if (!s) throw error{};
        return s;
    }

    // FIXME: With sequences you could actually prevent these indexing errors
    // from happening in the first place, instead of throwing when they happen.
    int non_portable_type() const
    {
        int result = SDL_SensorGetDeviceNonPortableType(index);
        if (result == -1) throw error{};
        return result;
    }

    // FIXME: We could avoid returning invalid if we made sure indexing errors
    // don't happen.
    sensor_type type() const noexcept
    {
        return static_cast<sensor_type>(SDL_SensorGetDeviceType(index));
    }
};

class sensor {
protected:
    SDL_Sensor* _handle = nullptr;

public:
    ~sensor()
    {
        if (_handle) SDL_SensorClose(_handle);
    }

    sensor() = default;

    sensor(sensor&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    sensor& operator=(sensor&& other) noexcept
    {
        if (_handle) SDL_SensorClose(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit sensor(SDL_Sensor* handle) noexcept
        : _handle{handle}
    {}

    SDL_Sensor* handle() const noexcept
    {
        return _handle;
    }

    explicit sensor(sensor_device device)
        : _handle{SDL_SensorOpen(device.index)}
    {
        if (!_handle) throw error{};
    }

    void get_data(span<float> data)
    {
        if (SDL_SensorGetData(_handle, data.data(), data.size()) < 0) {
            throw error{};
        }
    }

    sensor_id id() const noexcept
    {
        return SDL_SensorGetInstanceID(_handle);
    }

    const char* name() const noexcept
    {
        return SDL_SensorGetName(_handle);
    }

    int non_portable_type() const noexcept
    {
        return SDL_SensorGetNonPortableType(_handle);
    }

    sensor_type type() const noexcept
    {
        return static_cast<sensor_type>(SDL_SensorGetType(_handle));
    }

    static sensor_ref from(sensor_id) noexcept;

    static void update() noexcept
    {
        SDL_SensorUpdate();
    }

    friend bool operator==(const sensor& x, const sensor& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const sensor& x, const sensor& y) noexcept
    {
        return !(x == y);
    }
};

struct sensor_ref : sensor {
    ~sensor_ref()
    {
        _handle = nullptr;
    }

    sensor_ref(const sensor_ref& other) noexcept
        : sensor{other._handle}
    {}

    sensor_ref(const sensor& other) noexcept
        : sensor{other.handle()}
    {}

    explicit sensor_ref(SDL_Sensor* s) noexcept
        : sensor{s}
    {}

    SDL_Sensor* handle() const noexcept
    {
        return _handle;
    }

    explicit operator bool() const noexcept
    {
        return _handle;
    }
};

inline sensor_ref sensor::from(sensor_id id) noexcept
{
    return sensor_ref{SDL_SensorFromInstanceID(id)};
}

} // namespace sdl

#endif // SDLW_SENSOR_INCLUDED
