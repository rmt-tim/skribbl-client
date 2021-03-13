#pragma once

#include <type_traits>

#include <SDL_stdinc.h>

/* #define SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(enum_type)                       \ */
/*     constexpr enum_type operator|(enum_type lhs, enum_type rhs) noexcept    \ */
/*     {                                                                       \ */
/*         const auto l = ::sdl::detail::to_underlying(lhs);                   \ */
/*         const auto r = ::sdl::detail::to_underlying(rhs);                   \ */
/*         return static_cast<enum_type>(l | r);                               \ */
/*     }                                                                       \ */
/*                                                                             \ */
/*     constexpr enum_type& operator|=(enum_type& lhs, enum_type rhs) noexcept \ */
/*     {                                                                       \ */
/*         lhs = lhs | rhs;                                                    \ */
/*         return lhs;                                                         \ */
/*     }                                                                       \ */
/*                                                                             \ */
/*     constexpr enum_type operator&(enum_type lhs, enum_type rhs) noexcept    \ */
/*     {                                                                       \ */
/*         const auto l = ::sdl::detail::to_underlying(lhs);                   \ */
/*         const auto r = ::sdl::detail::to_underlying(rhs);                   \ */
/*         return static_cast<enum_type>(l & r);                               \ */
/*     }                                                                       \ */
/*                                                                             \ */
/*     constexpr enum_type& operator&=(enum_type& lhs, enum_type rhs) noexcept \ */
/*     {                                                                       \ */
/*         lhs = lhs & rhs;                                                    \ */
/*         return lhs;                                                         \ */
/*     } */

namespace sdl {
namespace detail {

template<typename Enum>
constexpr auto to_underlying(Enum value) noexcept
{
    static_assert(std::is_enum_v<Enum>);
    return static_cast<std::underlying_type_t<Enum>>(value);
}

/* template<auto F> */
/* struct make_functor { */
/*     template<typename... Args> */
/*     constexpr auto operator()(Args... args) noexcept(noexcept(F(std::forward<Args>(args)...))) */
/*     { */
/*         return F(std::forward<Args>(args)...); */
/*     } */
/* }; */

/* struct sdl_string_deleter { */
/*     void operator()(const char* mem) const noexcept */
/*     { */
/*         SDL_free(const_cast<char*>(mem)); */
/*     } */
/* }; */

struct sdl_deleter {
    void operator()(const char* mem) const noexcept
    {
        SDL_free(const_cast<char*>(mem));
    }
};

} // namespace detail
} // namespace sdl
