#ifndef SDLW_RECT_INCLUDED
#define SDLW_RECT_INCLUDED

#include <tuple>

#include <SDL_rect.h>

#include <sdlw/types.hpp>

namespace sdl {

struct point {
    int x;
    int y;
};

struct size {
    int width;
    int height;
};

struct rect {
    point position;
    size size;
};

struct line {
    point first;
    point second;
};

inline optional<rect> enclose_points(span<const point, int> points, const rect* clip) noexcept
{
    auto sdl_points = reinterpret_cast<const SDL_Point*>(points.data());
    auto sdl_points_size = static_cast<int>(points.size());
    auto sdl_clip = reinterpret_cast<const SDL_Rect*>(clip);
    rect result;
    auto sdl_result = reinterpret_cast<SDL_Rect*>(&result);
    SDL_Rect r;
    if (SDL_EnclosePoints(sdl_points, sdl_points_size, sdl_clip, sdl_result)) {
        return result;
    } else {
        return {};
    }
}

inline bool are_intersecting(const rect& r1, const rect& r2) noexcept
{
    auto a = reinterpret_cast<const SDL_Rect*>(&r1);
    auto b = reinterpret_cast<const SDL_Rect*>(&r2);
    return SDL_HasIntersection(a, b);
}

inline optional<rect> intersection(const rect& r1, const rect& r2) noexcept
{
    auto a = reinterpret_cast<const SDL_Rect*>(&r1);
    auto b = reinterpret_cast<const SDL_Rect*>(&r2);
    SDL_Rect result;
    if (SDL_IntersectRect(a, b, &result)) {
        return rect{result.x, result.y, result.w, result.h};
    } else {
        return {};
    }
}

inline optional<line> intersection(const rect& r, const line& l) noexcept
{
    auto rr = reinterpret_cast<const SDL_Rect*>(&r);
    line result = l;
    if (SDL_IntersectRectAndLine(rr, &result.first.x, &result.first.y, &result.second.x, &result.second.y)) {
        return result;
    } else {
        return {};
    }
}

inline bool is_point_in_rectangle(const point& p, const rect& r) noexcept
{
    auto pp = reinterpret_cast<const SDL_Point*>(&p);
    auto rr = reinterpret_cast<const SDL_Rect*>(&r);
    return SDL_PointInRect(pp, rr);
}

inline bool is_empty(const rect& r) noexcept
{
    auto rr = reinterpret_cast<const SDL_Rect*>(&r);
    return SDL_RectEmpty(rr);
}

inline rect rect_union(const rect& r1, const rect& r2) noexcept
{
    auto a = reinterpret_cast<const SDL_Rect*>(&r1);
    auto b = reinterpret_cast<const SDL_Rect*>(&r2);
    SDL_Rect result;
    SDL_UnionRect(a, a, &result);
    return rect{result.x, result.x, result.w, result.h};
}

} // namespace sdl

#endif // SDLW_RECT_INCLUDED
