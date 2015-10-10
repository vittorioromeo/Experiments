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
        namespace impl
        {
            struct vao
            {
                GLuint _id, _n;

                vao() = default;
                vao(GLuint n) noexcept : _n{n} {}

                void generate()
                {
                    VRM_SDL_GLCHECK(glGenVertexArraysOES(_n, &_id));
                }
                void deleteVAO()
                {
                    VRM_SDL_GLCHECK(glDeleteVertexArraysOES(_n, &_id));
                }
                void bind()
                {
                    // std::cout << "bound vao " << _id << "\n";
                    VRM_SDL_GLCHECK(glBindVertexArrayOES(_id));
                }
                void unbind() { VRM_SDL_GLCHECK(glBindVertexArrayOES(0)); }

                template <typename TF>
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

        auto make_vao(GLuint n)
        {
            impl::vao v{n};
            v.generate();

            return impl::unique_vao{v};
        }
    }
}