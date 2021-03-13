#ifndef SDLW_CPU_INFO_INCLUDED
#define SDLW_CPU_INFO_INCLUDED

#include <SDL_cpuinfo.h>

namespace sdl {

// clang-format off

inline int  cache_line_size() noexcept { return SDL_GetCPUCacheLineSize(); }
inline int  cpu_count()       noexcept { return SDL_GetCPUCount();         }
inline int  system_ram()      noexcept { return SDL_GetSystemRAM();        }
inline bool has_3dnow()       noexcept { return SDL_Has3DNow();            }
inline bool has_avx()         noexcept { return SDL_HasAVX();              }
inline bool has_avx2()        noexcept { return SDL_HasAVX2();             }
inline bool has_altivec()     noexcept { return SDL_HasAltiVec();          }
inline bool has_mmx()         noexcept { return SDL_HasMMX();              }
inline bool has_rdtsc()       noexcept { return SDL_HasRDTSC();            }
inline bool has_sse()         noexcept { return SDL_HasSSE();              }
inline bool has_sse2()        noexcept { return SDL_HasSSE2();             }
inline bool has_sse3()        noexcept { return SDL_HasSSE3();             }
inline bool has_sse41()       noexcept { return SDL_HasSSE41();            }
inline bool has_sse42()       noexcept { return SDL_HasSSE42();            }

// clang-format on

} // namespace sdl

#endif // SDLW_CPU_INFO_INCLUDED
