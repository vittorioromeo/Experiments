// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cassert>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/math/trig_table.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        static constexpr sz_t trig_table_precision{628};

        VRM_SDL_ALWAYS_INLINE const auto& sin_table() noexcept
        {
            static trig_table<trig_table_precision> result{[](auto x)
                {
                    return std::sin(x);
                }};

            return result;
        }

        VRM_SDL_ALWAYS_INLINE const auto& cos_table() noexcept
        {
            static trig_table<trig_table_precision> result{[](auto x)
                {
                    return std::cos(x);
                }};

            return result;
        }
    }

    VRM_SDL_ALWAYS_INLINE auto tbl_sin(float mX) noexcept
    {
        return impl::sin_table().get(mX);
    }

    VRM_SDL_ALWAYS_INLINE auto tbl_cos(float mX) noexcept
    {
        return impl::cos_table().get(mX);
    }
}
VRM_SDL_NAMESPACE_END
