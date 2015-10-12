// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ostream>
#include <vrm/sdl/dependencies.hpp>

namespace vrm
{
    namespace sdl
    {
        template <typename T>
        using vec2 = glm::tvec2<T>;

        using vec2f = vec2<float>;
        using vec2i = vec2<int>;
        using vec2u = vec2<unsigned int>;

        template <typename T0, typename T1>
        auto make_vec2(T0 x, T1 y) noexcept
        {
            return vec2<std::common_type_t<T0, T1>>(x, y);
        }

        template <typename T>
        auto& operator<<(std::ostream& o, const vec2<T>& v)
        {
            o << "(" << v.x << ", " << v.y << ")";
            return o;
        }
    }
}