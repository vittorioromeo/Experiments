// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        constexpr float pi{3.14159265f};
        constexpr float pi_half{pi / 2.f};
        constexpr float tau{pi * 2.f};
        constexpr float rad_deg_ratio{pi / 180.f};

        template <typename T>
        constexpr auto to_rad(const T& x) noexcept
        {
            return x * rad_deg_ratio;
        }

        template <typename T>
        constexpr auto to_deg(const T& x) noexcept
        {
            return x / rad_deg_ratio;
        }
    }
}