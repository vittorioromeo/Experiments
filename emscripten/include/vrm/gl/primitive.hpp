// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/gl/common.hpp>

VRM_SDL_NAMESPACE
{
    enum class primitive : GLenum
    {
        points = GL_POINTS,
        lines = GL_LINES,
        line_strip = GL_LINE_STRIP,
        line_loop = GL_LINE_LOOP,
        triangles = GL_TRIANGLES,
        triangle_strip = GL_TRIANGLE_STRIP,
        triangle_fan = GL_TRIANGLE_FAN
    };
}
VRM_SDL_NAMESPACE_END