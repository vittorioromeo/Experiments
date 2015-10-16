// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>

VRM_SDL_NAMESPACE
{
    enum class primitive
    {
        points,
        lines,
        line_strip,
        line_loop,
        triangles,
        triangle_strip,
        triangle_fan
    };

    namespace impl
    {
        template <primitive TP>
        constexpr GLenum primitive_value{GL_POINTS};

        template <>
        constexpr GLenum primitive_value<primitive::lines>{GL_LINES};

        template <>
        constexpr GLenum primitive_value<primitive::line_strip>{GL_LINE_STRIP};

        template <>
        constexpr GLenum primitive_value<primitive::line_loop>{GL_LINE_LOOP};

        template <>
        constexpr GLenum primitive_value<primitive::triangles>{GL_TRIANGLES};

        template <>
        constexpr GLenum primitive_value<primitive::triangle_strip>{
            GL_TRIANGLE_STRIP};

        template <>
        constexpr GLenum primitive_value<primitive::triangle_fan>{
            GL_TRIANGLE_FAN};
    }
}
VRM_SDL_NAMESPACE_END