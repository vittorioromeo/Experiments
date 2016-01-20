// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cassert>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/math/sin_cos_table.hpp>
#include <vrm/sdl/math/vec.hpp>
#include <vrm/sdl/math/mat.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        template <typename TFX, typename TFY>
        auto ratio_scale_impl(const vec2f& original_size,
            const vec2f& container_size, TFX&& step_x_fn,
            TFY&& step_y_fn) noexcept
        {
            const auto& os(original_size);
            const auto& ws(container_size);
            auto original_ratio(os.x / os.y);

            if(ws.y * original_ratio <= ws.x)
            {
                // the width is the boss

                auto r_width = ws.y * original_ratio;
                step_x_fn(r_width);
                return vec2f(r_width, r_width / original_ratio);
            }
            else
            {
                // the height is the boss

                auto r_height = ws.x / original_ratio;
                step_y_fn(r_height);
                return vec2f(r_height * original_ratio, r_height);
            }
        }

        auto discrete_ratio_step(float value, float increment) noexcept
        {
            return std::floor(value / increment) * increment;
        }

        auto discrete_ratio_scale(const vec2f& original_size,
            const vec2f& container_size, float x_increment,
            float y_increment) noexcept
        {
            return ratio_scale_impl(original_size, container_size,
                [=](auto& x)
                {
                    x = discrete_ratio_step(x, x_increment);
                },
                [=](auto& y)
                {
                    y = discrete_ratio_step(y, y_increment);
                });
        }

        auto ratio_scale(
            const vec2f& original_size, const vec2f& container_size) noexcept
        {
            return ratio_scale_impl(original_size, container_size,
                [](auto&)
                {
                },
                [](auto&)
                {
                });
        }

        auto ratio_scale_margin(
            const vec2f& scaled_size, const vec2f& container_size) noexcept
        {
            return (container_size - scaled_size) / 2.f;
        }
    }
}
VRM_SDL_NAMESPACE_END