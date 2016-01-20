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

    template <typename T0, typename T1>
    VRM_SDL_ALWAYS_INLINE auto make_vec(const T0& x, const T1& y) noexcept
    {
        return vec2<std::common_type_t<T0, T1>>(x, y);
    }

    template <typename T0, typename T1, typename T2>
    VRM_SDL_ALWAYS_INLINE auto make_vec(
        const T0& x, const T1& y, const T2& z) noexcept
    {
        return vec3<std::common_type_t<T0, T1, T2>>(x, y, z);
    }

    template <typename T0, typename T1, typename T2, typename T3>
    VRM_SDL_ALWAYS_INLINE auto make_vec(
        const T0& x, const T1& y, const T2& z, const T3& w) noexcept
    {
        return vec4<std::common_type_t<T0, T1, T2, T3>>(x, y, z, w);
    }
}
VRM_SDL_NAMESPACE_END

template <typename T>
auto& operator<<(std::ostream& o, const vrm::sdl::vec2<T>& v)
{
    o << "(" << v.x << ", " << v.y << ")";
    return o;
}

template <typename T>
auto& operator<<(std::ostream& o, const vrm::sdl::vec3<T>& v)
{
    o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return o;
}

template <typename T>
auto& operator<<(std::ostream& o, const vrm::sdl::vec4<T>& v)
{
    o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return o;
}