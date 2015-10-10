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
            constexpr std::size_t shaderLogBufferSize{512};
            static char logBuffer[shaderLogBufferSize];
            static std::string logString;

            auto id(glCreateShader(type));
            glShaderSource(id, 1, src, nullptr);
            glCompileShader(id);

            GLint status;
            glGetShaderiv(id, GL_COMPILE_STATUS, &status);


            glGetShaderInfoLog(id, shaderLogBufferSize, nullptr, logBuffer);
            logString = logBuffer;

            if(!logString.empty())
                std::cout << "Shader compilation log" << logString << std::endl;

            return impl::unique_shader{id};
        }

        // TODO: move to correct file


        namespace impl
        {
            struct vao
            {
                GLuint _id, _n;

                vao() = default;
                vao(GLuint n) noexcept : _n{n} {}

                void generate() { glGenVertexArraysOES(_n, &_id); }
                void deleteVAO() { glDeleteVertexArraysOES(_n, &_id); }
                void bind()
                {
                    // std::cout << "bound vao " << _id << "\n";
                    glBindVertexArrayOES(_id);
                }
                void unbind() { glBindVertexArrayOES(0); }

                template<typename TF>
                void with(TF&& f)
                {
                    bind();
                    f();
                    unbind();
                }
            };

            struct gl_vao_deleter
            {
                void operator()(impl::vao& vao) noexcept
                {
                    // std::cout << "vao deleted\n";
                    vao.deleteVAO();
                }
            };

            using unique_vao = unique_resource<impl::vao, gl_vao_deleter>;
        }

        namespace impl
        {
            struct vbo
            {
                GLuint _id, _n;

                vbo() = default;
                vbo(GLuint n) noexcept : _n{n} {}

                void generate() { glGenBuffers(_n, &_id); }
                void deleteVBO() { glDeleteBuffers(_n, &_id); }
                void bind(GLenum target)
                {
                    // std::cout << "bound vbo " << _id << "\n";
                    glBindBuffer(target, _id);
                }
                void unbind() { glBindBuffer(0, 0); }

                template <typename T>
                void buffer_data(
                    GLenum target, GLenum usage, T* data, sz_t count)
                {
                    glBufferData(target, count * sizeof(T), data, usage);
                }

                template <typename T, sz_t TN>
                void buffer_data(GLenum target, GLenum usage, T(&arr)[TN])
                {
                    buffer_data(target, usage, arr, TN);
                }

                template<typename TF>
                void with(GLenum target, TF&& f)
                {
                    this->bind(target);
                    f();
                    unbind();
                }
            };

            struct gl_vbo_deleter
            {
                void operator()(impl::vbo& vbo) noexcept
                {
                    // std::cout << "vbo deleted\n";
                    vbo.deleteVBO();
                }
            };

            using unique_vbo = unique_resource<impl::vbo, gl_vbo_deleter>;
        }

        auto make_vao(GLuint n)
        {
            impl::vao v{n};
            v.generate();

            return impl::unique_vao{v};
        }

        auto make_vbo(GLuint n)
        {
            impl::vbo v{n};
            v.generate();

            return impl::unique_vbo{v};
        }
    }
}