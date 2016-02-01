// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/math.hpp>

VRM_SDL_NAMESPACE
{
    namespace window_scale
    {
        auto VRM_CORE_CONST_FN fixed() noexcept
        {
            return [](const vec2f& original_size, const vec2f&)
            {
                return original_size;
            };
        }

        auto VRM_CORE_CONST_FN stretch() noexcept
        {
            return [](const vec2f&, const vec2f& window_size)
            {
                return window_size;
            };
        }

        auto VRM_CORE_CONST_FN ratio_aware() noexcept
        {
            return [](const vec2f& original_size, const vec2f& window_size)
            {
                return impl::ratio_scale(original_size, window_size);
            };
        }

        auto VRM_CORE_CONST_FN discrete_ratio_aware(
            const vec2f& increment) noexcept
        {
            return [=](const vec2f& original_size, const vec2f& window_size)
            {
                return impl::discrete_ratio_scale(
                    original_size, window_size, increment.x, increment.y);
            };
        }

        auto VRM_CORE_CONST_FN pixel_perfect() noexcept
        {
            return [=](const vec2f& original_size, const vec2f& window_size)
            {
                return discrete_ratio_aware(original_size)(
                    original_size, window_size);
            };
        }
    }
}
VRM_SDL_NAMESPACE_END
