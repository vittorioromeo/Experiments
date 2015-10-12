// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/check.hpp>
#include <vrm/sdl/gl/primitive.hpp>

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            struct vao
            {
                GLuint _id, _n;

                vao() = default;
                vao(GLuint n) noexcept : _n{n} {}

                void generate() noexcept
                {
                    VRM_SDL_GLCHECK(glGenVertexArraysOES(_n, &_id));
                }
                void deleteVAO() noexcept
                {
                    VRM_SDL_GLCHECK(glDeleteVertexArraysOES(_n, &_id));
                }
                void bind() noexcept
                {
                    // std::cout << "bound vao " << _id << "\n";
                    VRM_SDL_GLCHECK(glBindVertexArrayOES(_id));
                }
                void unbind() noexcept
                {
                    VRM_SDL_GLCHECK(glBindVertexArrayOES(0));
                }

                template <typename TF>
                void with(TF&& f) noexcept
                {
                    bind();
                    f();
                    unbind();
                }

                template <primitive TP>
                void draw_arrays(GLint first, GLsizei count) noexcept
                {
                    VRM_SDL_GLCHECK(
                        glDrawArrays(impl::primitive_value<TP>, first, count));
                }

                template <primitive TP>
                void with_draw_arrays(GLint first, GLsizei count) noexcept
                {
                    with([this, first, count]
                        {
                            draw_arrays<TP>(first, count);
                        });
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

        auto make_vao(GLuint n) noexcept
        {
            impl::vao v{n};
            v.generate();

            return impl::unique_vao{v};
        }
    }
}