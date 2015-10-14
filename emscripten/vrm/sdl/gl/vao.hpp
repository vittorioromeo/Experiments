// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/dependencies.hpp>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/check.hpp>
#include <vrm/sdl/gl/primitive.hpp>
#include <vrm/sdl/gl/index_type.hpp>

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
                void draw_arrays(
                    GLint first_index, GLsizei index_count) noexcept
                {
                    VRM_SDL_GLCHECK(glDrawArrays(
                        impl::primitive_value<TP>, first_index, index_count));
                }

                template <primitive TP, index_type TI>
                void draw_elements(
                    GLsizei index_count, sz_t vbo_offset_byte = 0) noexcept
                {
                    /*
                    std::cout
                        << "glDrawElements(impl::primitive_value<TP>, "
                           "index_count= "
                        << index_count
                        << ", impl::index_type_value<TI>, vbo_offset_byte= "
                        << vbo_offset_byte << "\n";
                    */

                    VRM_SDL_GLCHECK(glDrawElements(impl::primitive_value<TP>,
                        index_count, impl::index_type_value<TI>,
                        (const void*)vbo_offset_byte));
                }

                template <primitive TP, typename... Ts>
                void with_draw_arrays(Ts&&... xs) noexcept
                {
                    with([this, &xs...]
                        {
                            draw_arrays<TP>(FWD(xs)...);
                        });
                }

                template <primitive TP, typename... Ts>
                void with_draw_elements(Ts&&... xs) noexcept
                {
                    with([this, &xs...]
                        {
                            draw_elements<TP>(FWD(xs)...);
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