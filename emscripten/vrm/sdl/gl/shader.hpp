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
        enum class shader_t
        {
            vertex,
            fragment
        };

        namespace impl
        {
            template <shader_t TV>
            constexpr GLenum shader_t_value{0};

            template <>
            constexpr GLenum shader_t_value<shader_t::vertex>{GL_VERTEX_SHADER};

            template <>
            constexpr GLenum shader_t_value<shader_t::fragment>{
                GL_FRAGMENT_SHADER};
        }

        auto make_shader(GLenum type, const GLchar** src) noexcept
        {
            constexpr std::size_t shaderLogBufferSize{512};
            static char logBuffer[shaderLogBufferSize];
            static std::string logString;

            GLuint id;

            VRM_SDL_GLCHECK(id = glCreateShader(type););
            VRM_SDL_GLCHECK(glShaderSource(id, 1, src, nullptr));
            VRM_SDL_GLCHECK(glCompileShader(id));

            GLint status;
            VRM_SDL_GLCHECK(glGetShaderiv(id, GL_COMPILE_STATUS, &status));


            VRM_SDL_GLCHECK(glGetShaderInfoLog(
                id, shaderLogBufferSize, nullptr, logBuffer));
            logString = logBuffer;

            if(!logString.empty())
                std::cout << "Shader compilation log" << logString << std::endl;

            return impl::unique_shader{id};
        }

        auto make_shader_from_file(
    }
}