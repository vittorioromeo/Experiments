// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cassert>
#include <vrm/sdl/common.hpp>

VRM_SDL_NAMESPACE
{
    constexpr float pi{3.14159265f};
    constexpr float pi_half{pi / 2.f};
    constexpr float tau{pi * 2.f};
    constexpr float rad_deg_ratio{pi / 180.f};

    template <typename T>
    constexpr VRM_SDL_ALWAYS_INLINE auto to_rad(const T& x) noexcept
    {
        return x * rad_deg_ratio;
    }

    template <typename T>
    constexpr VRM_SDL_ALWAYS_INLINE auto to_deg(const T& x) noexcept
    {
        return x / rad_deg_ratio;
    }

    template <typename T>
    VRM_SDL_ALWAYS_INLINE auto wrap_rad(const T& x) noexcept
    {
        assert(x >= -sdl::tau && x <= 2.f * sdl::tau);

        if(x < 0) return x + sdl::tau;
        if(x > sdl::tau) return x - sdl::tau;

        return x;
    }
}
VRM_SDL_NAMESPACE_END