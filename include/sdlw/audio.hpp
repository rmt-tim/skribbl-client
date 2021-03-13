#ifndef SDLW_AUDIO_INCLUDED
#define SDLW_AUDIO_INCLUDED

#include <SDL_audio.h>

#include <sdlw/error.hpp>
#include <sdlw/rwops.hpp>
#include <sdlw/types.hpp>

namespace sdl {

using audio_device_id = u32;

enum class audio_status : typename std::underlying_type<SDL_AudioStatus>::type {
    stopped = SDL_AUDIO_STOPPED,
    playing = SDL_AUDIO_PLAYING,
    paused  = SDL_AUDIO_PAUSED
};

struct audio_driver {
    int index;

    const char* name() const
    {
        const char* s = SDL_GetAudioDriver(index);
        if (!s) throw error{};
        return s;
    }

    static const char* current() noexcept
    {
        return SDL_GetCurrentAudioDriver();
    }

    static int count() noexcept
    {
        return SDL_GetNumAudioDrivers();
    }
};

struct audio_subsystem {
    audio_subsystem(const char* driver_name)
    {
        if (SDL_AudioInit(driver_name) != 0) throw error{};
    }

    ~audio_subsystem()
    {
        SDL_AudioQuit();
    }
};

struct audio_device {
    int index;

    const char* name(bool is_capture) const
    {
        const char* s = SDL_GetAudioDeviceName(index, static_cast<int>(is_capture));
        if (!s) throw error{};
        return s;
    }

    static int count(bool is_capture) noexcept
    {
        return SDL_GetNumAudioDevices(static_cast<int>(is_capture));
    }
};

enum class audio_format : SDL_AudioFormat {
    s8     = AUDIO_S8,
    u8     = AUDIO_U8,
    s16lsb = AUDIO_S16LSB,
    s16msb = AUDIO_S16MSB,
    s16sys = AUDIO_S16SYS,
    s16    = AUDIO_S16,
    u16lsb = AUDIO_U16LSB,
    u16msb = AUDIO_U16MSB,
    u16sys = AUDIO_U16SYS,
    u16    = AUDIO_U16,
    s32lsb = AUDIO_S32LSB,
    s32msb = AUDIO_S32MSB,
    s32sys = AUDIO_S32SYS,
    s32    = AUDIO_S32,
    f32lsb = AUDIO_F32LSB,
    f32msb = AUDIO_F32MSB,
    f32sys = AUDIO_F32SYS,
    f32    = AUDIO_F32,
};

constexpr int bit_size(audio_format f)
{
    return SDL_AUDIO_BITSIZE(static_cast<SDL_AudioFormat>(f));
}

constexpr bool is_float(audio_format f)
{
    return static_cast<bool>(SDL_AUDIO_ISFLOAT(static_cast<SDL_AudioFormat>(f)));
}

constexpr bool is_big_endian(audio_format f)
{
    return static_cast<bool>(SDL_AUDIO_ISBIGENDIAN(static_cast<SDL_AudioFormat>(f)));
}

constexpr bool is_signed(audio_format f)
{
    return static_cast<bool>(SDL_AUDIO_ISSIGNED(static_cast<SDL_AudioFormat>(f)));
}

constexpr bool is_int(audio_format f)
{
    return static_cast<bool>(SDL_AUDIO_ISINT(static_cast<SDL_AudioFormat>(f)));
}

constexpr bool is_little_endian(audio_format f)
{
    return static_cast<bool>(SDL_AUDIO_ISLITTLEENDIAN(static_cast<SDL_AudioFormat>(f)));
}

constexpr bool is_unsigned(audio_format f)
{
    return static_cast<bool>(SDL_AUDIO_ISUNSIGNED(static_cast<SDL_AudioFormat>(f)));
}

struct audio_callback {
    SDL_AudioCallback _callback = nullptr;
    void* _userdata = nullptr;

    audio_callback() = default;

    audio_callback(void (*callback)(span<u8>)) noexcept
    {
        _callback = [] (void* userdata, u8* stream, int len) -> void {
            reinterpret_cast<void(*)(span<u8>)>(userdata)(span<u8>{stream, len});
        };
        _userdata = reinterpret_cast<void*>(callback);
    }

    template <class Callback>
    audio_callback(Callback& callback) noexcept
    {
        _callback = [] (void* userdata, u8* stream, int len) -> void {
            (*reinterpret_cast<Callback*>(userdata))(span<u8>{stream, len});
        };
        _userdata = &callback;
    }
};

struct audio_spec {
    int frequency;
    audio_format format;
    u8 channels;
    u8 silence;
    u16 samples;
    u16 _padding;
    u32 size;
    audio_callback callback;
};

class audio {
    SDL_AudioDeviceID _handle = 0;

public:
    ~audio()
    {
        if (_handle) SDL_CloseAudioDevice(_handle);
    }

    audio() = default;

    audio(audio&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = 0;
    }

    audio& operator=(audio&& other) noexcept
    {
        if (_handle) SDL_CloseAudioDevice(_handle);
        _handle = other._handle;
        other._handle = 0;
        return *this;
    }

    explicit audio(SDL_AudioDeviceID handle) noexcept
        : _handle{handle}
    {}

    SDL_AudioDeviceID handle() const noexcept
    {
        return _handle;
    }

    enum allowed_changes {
        allow_frequency_change = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE,
        allow_format_change    = SDL_AUDIO_ALLOW_FORMAT_CHANGE,
        allow_channels_change  = SDL_AUDIO_ALLOW_CHANNELS_CHANGE,
        allow_any_change       = SDL_AUDIO_ALLOW_ANY_CHANGE
    };

    audio(const char* device, bool iscapture, const audio_spec& desired, audio_spec& obtained, int allowed_changes)
        : _handle{SDL_OpenAudioDevice(device, static_cast<int>(iscapture), (const SDL_AudioSpec*)&desired, (SDL_AudioSpec*)&obtained, allowed_changes)}
    {
        if (!_handle) throw error{};
    }

    void lock() noexcept
    {
        SDL_LockAudioDevice(_handle);
    }

    void unlock() noexcept
    {
        SDL_UnlockAudioDevice(_handle);
    }

    audio_status status() const noexcept
    {
        return static_cast<audio_status>(SDL_GetAudioDeviceStatus(_handle));
    }

    void play() noexcept
    {
        SDL_PauseAudioDevice(_handle, 0);
    }

    void pause() noexcept
    {
        SDL_PauseAudioDevice(_handle, 1);
    }

    void queue(span<const u8, u32> audio)
    {
        if (SDL_QueueAudio(_handle, audio.data(), audio.size()) < 0) {
            throw error{};
        }
    }

    u32 dequeue(span<u8, u32> audio)
    {
        u32 result = SDL_DequeueAudio(_handle, audio.data(), audio.size());
        if (result < 0) throw error{};
        return result;
    }

    void clear_queued() noexcept
    {
        SDL_ClearQueuedAudio(_handle);
    }

    u32 queued_size() const noexcept
    {
        return SDL_GetQueuedAudioSize(_handle);
    }

    friend bool operator==(const audio& x, const audio& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const audio& x, const audio& y) noexcept
    {
        return !(x == y);
    }
};

struct wav_deleter {
    void operator()(u8* audio_buffer) const
    {
        SDL_FreeWAV(audio_buffer);
    }
};

struct wav {
    array<u8, u32, wav_deleter> audio_buffer;
    audio_spec spec;
};

inline wav load_wav(const char* file)
{
    u8* audio_buffer = nullptr;
    u32 audio_length = 0;
    audio_spec spec;
    if (SDL_LoadWAV(file, (SDL_AudioSpec*)&spec, &audio_buffer, &audio_length) == nullptr) {
        throw error{};
    }
    return wav{{audio_buffer, audio_length}, spec};
}

inline wav load_wav(stream& source)
{
    u8* audio_buffer = nullptr;
    u32 audio_length = 0;
    audio_spec spec;
    if (SDL_LoadWAV_RW(source.get_pointer(), 0, (SDL_AudioSpec*)&spec, &audio_buffer, &audio_length) == nullptr) {
        throw error{};
    }
    return wav{{audio_buffer, audio_length}, spec};
}

// TODO: SDL_MIX_MAXVOLUME

inline void mix_audio(span<const u8, u32> source, span<u8, u32> destination, audio_format format, int volume) noexcept
{
    SDL_MixAudioFormat(destination.data(), source.data(), static_cast<SDL_AudioFormat>(format), destination.size(), volume);
}

struct audio_cvt {
    int needed;
    audio_format source_format;
    audio_format destination_format;
    double rate_conversion_increment;
    u8* buffer;
    int length;
    int len_cvt;
    int len_mult;
    int len_ratio;
    SDL_AudioFilter _filters[10];
    int _filter_index;
};

inline optional<audio_cvt> build_audio_cvt(audio_format src_format, u8 src_channels, int src_rate,
                                                audio_format dst_format, u8 dst_channels, int dst_rate) noexcept
{
    audio_cvt cvt;
    if (SDL_BuildAudioCVT((SDL_AudioCVT*)&cvt,
                          (SDL_AudioFormat)src_format, src_channels, src_rate,
                          (SDL_AudioFormat)dst_format, dst_channels, dst_rate)) {
        return cvt;
    } else {
        return {};
    }
}

inline void convert_audio(audio_cvt& cvt)
{
    if (SDL_ConvertAudio((SDL_AudioCVT*)&cvt) < 0) {
        throw error{};
    }
}

class audio_stream {
    SDL_AudioStream* _handle = nullptr;

public:
    ~audio_stream()
    {
        if (_handle) SDL_FreeAudioStream(_handle);
    }

    audio_stream() = default;

    audio_stream(audio_stream&& other) noexcept
        : _handle{other._handle}
    {
        other._handle = nullptr;
    }

    audio_stream& operator=(audio_stream&& other) noexcept
    {
        if (_handle) SDL_FreeAudioStream(_handle);
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    explicit audio_stream(SDL_AudioStream* handle) noexcept
        : _handle{handle}
    {}

    SDL_AudioStream* handle() const noexcept
    {
        return _handle;
    }

    audio_stream(audio_format src_format, u8 src_channels, int src_rate, audio_format dst_format, u8 dst_channels, int dst_rate)
        : _handle{SDL_NewAudioStream((SDL_AudioFormat)src_format, src_channels, src_rate, (SDL_AudioFormat)dst_format, dst_channels, dst_rate)}
    {
        if (!_handle) throw error{};
    }

    void put(span<const u8> buffer)
    {
        if (SDL_AudioStreamPut(_handle, buffer.data(), buffer.size()) < 0) {
            throw error{};
        }
    }

    int get(span<u8> buffer)
    {
        int result = SDL_AudioStreamGet(_handle, buffer.data(), buffer.size());
        if (result < 0) throw error{};
        return result;
    }

    int available() const noexcept
    {
        return SDL_AudioStreamAvailable(_handle);
    }

    // TODO: Return value meaning undocumented.
    int flush() noexcept
    {
        return SDL_AudioStreamFlush(_handle);
    }

    void clear() noexcept
    {
        SDL_AudioStreamClear(_handle);
    }

    friend bool operator==(const audio_stream& x, const audio_stream& y) noexcept
    {
        return x.handle() == y.handle();
    }

    friend bool operator!=(const audio_stream& x, const audio_stream& y) noexcept
    {
        return !(x == y);
    }
};

} // namespace sdl

#endif // SDLW_AUDIO_INCLUDED
