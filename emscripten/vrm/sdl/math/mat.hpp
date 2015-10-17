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
}
VRM_SDL_NAMESPACE_END

template <typename T>
auto& operator<<(std::ostream& o, const vrm::sdl::mat2<T>& v)
{
    o << "{" << v[0] << "\n" << v[1] << "}";
    return o;
}

template <typename T>
auto& operator<<(std::ostream& o, const vrm::sdl::mat3<T>& v)
{
    o << "{" << v[0] << "\n" << v[1] << "\n" << v[2] << "}";
    return o;
}

template <typename T>
auto& operator<<(std::ostream& o, const vrm::sdl::mat4<T>& v)
{
    o << "{" << v[0] << "\n" << v[1] << "\n" << v[2] << "\n" << v[3] << "}";
    return o;
}