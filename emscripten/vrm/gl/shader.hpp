// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <fstream>
#include <vrm/gl/common.hpp>
#include <vrm/gl/check.hpp>

VRM_SDL_NAMESPACE
{
    enum class shader_t : GLenum
    {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER
    };

    namespace impl
    {
        struct gl_shader_deleter
        {
            void operator()(GLint id) noexcept { glDeleteShader(id); }
        };

        using unique_shader = unique_resource<GLint, gl_shader_deleter>;
    }

    template <shader_t TS>
    auto make_shader(const char* src) noexcept
    {
        constexpr sz_t shaderLogBufferSize{512};
        static char logBuffer[shaderLogBufferSize];
        static std::string logString;

        GLuint id;

        VRM_SDL_GLCHECK(id = glCreateShader(vrmc::from_enum(TS)););
        VRM_SDL_GLCHECK(glShaderSource(id, 1, &src, nullptr));
        VRM_SDL_GLCHECK(glCompileShader(id));

        GLint status;
        VRM_SDL_GLCHECK(glGetShaderiv(id, GL_COMPILE_STATUS, &status));

        VRM_SDL_GLCHECK(
            glGetShaderInfoLog(id, shaderLogBufferSize, nullptr, logBuffer));

        logString = logBuffer;

        if(!logString.empty())
        {
            std::cout << "Shader compilation log" << logString << std::endl;
            int temp;
            std::cin >> temp;
        }

        return impl::unique_shader{id};
    }

    template <shader_t TS>
    auto make_shader_from_file(const std::string& path)
    {
        std::ifstream ifs{path.c_str(), std::ios_base::binary};
        VRM_CORE_ASSERT(!ifs.fail());

        auto f_begin(ifs.tellg());
        ifs.seekg(0, std::ios::end);
        auto size(ifs.tellg());
        ifs.seekg(0, std::ios::beg);

        auto buffer(std::make_unique<char[]>(size - f_begin + 1));
        ifs.read(&buffer[0], size);
        buffer[size] = '\0';

        return make_shader<TS>(buffer.get());
    }
}
VRM_SDL_NAMESPACE_END