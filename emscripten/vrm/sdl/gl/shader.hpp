// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>

namespace vrm
{
    namespace sdl
    {
        auto make_shader(GLenum type, const GLchar** src) noexcept
        {
            /*
            constexpr std::size_t shaderLogBufferSize{512};
            thread_local char logBuffer[shaderLogBufferSize];
            thread_local std::string logString;
            */

            auto id(glCreateShader(type));
            glShaderSource(id, 1, src, nullptr);
            glCompileShader(id);

            // GLint status;
            // glGetShaderiv(id, GL_COMPILE_STATUS, &status);

            /*
            glGetShaderInfoLog(id, shaderLogBufferSize, nullptr, logBuffer);
            logString = logBuffer;

            if(!logString.empty())
                std::cout << "Shader compilation log" << logString << std::endl;
            */

            return impl::unique_shader{id};
        }
    }
}