// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ostream>
#include <vrm/sdl/dependencies.hpp>

VRM_SDL_NAMESPACE
{
    template <typename T>
    using vec2 = glm::tvec2<T>;

    template <typename T>
    using vec3 = glm::tvec3<T>;

    template <typename T>
    using vec4 = glm::tvec4<T>;

    using vec2d = vec2<double>;
    using vec2f = vec2<float>;
    using vec2i = vec2<int>;
    using vec2u = vec2<unsigned int>;

    using vec3d = vec3<double>;
    using vec3f = vec3<float>;
    using vec3i = vec3<int>;
    using vec3u = vec3<unsigned int>;

    using vec4d = vec4<double>;
    using vec4f = vec4<float>;
    using vec4i = vec4<int>;
    using vec4u = vec4<unsigned int>;





    template <typename T>
    using mat2 = glm::tmat2x2<T>;

    template <typename T>
    using mat3 = glm::tmat3x3<T>;

    template <typename T>
    using mat4 = glm::tmat4x4<T>;

    using mat2d = mat2<double>;
    using mat2f = mat2<float>;
    using mat2i = mat2<int>;
    using mat2u = mat2<unsigned int>;

    using mat3d = mat3<double>;
    using mat3f = mat3<float>;
    using mat3i = mat3<int>;
    using mat3u = mat3<unsigned int>;

    using mat4d = mat4<double>;
    using mat4f = mat4<float>;
    using mat4i = mat4<int>;
    using mat4u = mat4<unsigned int>;





    template <typename T0, typename T1>
    auto make_vec2(T0 x, T1 y) noexcept
    {
        return vec2<std::common_type_t<T0, T1>>(x, y);
    }

    template <typename T>
    auto& operator<<(std::ostream & o, const vec2<T>& v)
    {
        o << "(" << v.x << ", " << v.y << ")";
        return o;
    }
}
VRM_SDL_NAMESPACE_END