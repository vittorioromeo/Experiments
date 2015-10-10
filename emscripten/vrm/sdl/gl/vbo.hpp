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
            template <GLenum TTarget>
            struct vbo
            {
                GLuint _id, _n;

                vbo() = default;
                vbo(GLuint n) noexcept : _n{n} {}

                void generate() { VRM_SDL_GLCHECK(glGenBuffers(_n, &_id)); }
                void deleteVBO() { VRM_SDL_GLCHECK(glDeleteBuffers(_n, &_id)); }
                void bind()
                {
                    // std::cout << "bound vbo " << _id << "\n";
                    VRM_SDL_GLCHECK(glBindBuffer(TTarget, _id));
                }
                void unbind() { VRM_SDL_GLCHECK(glBindBuffer(TTarget, 0)); }

                template <typename T>
                void buffer_data(GLenum usage, T* data, sz_t count)
                {
                    VRM_SDL_GLCHECK(
                        glBufferData(TTarget, count * sizeof(T), data, usage));
                }

                template <typename T, sz_t TN>
                void buffer_data(GLenum usage, T(&arr)[TN])
                {
                    buffer_data(usage, &arr[0], TN);
                }

                template <typename TF>
                void with(TF&& f)
                {
                    bind();
                    f();
                    unbind();
                }
            };

            struct gl_vbo_deleter
            {
                template <typename T>
                void operator()(T& vbo) noexcept
                {
                    // std::cout << "vbo deleted\n";
                    vbo.deleteVBO();
                }
            };

            template <GLenum TTarget>
            using unique_vbo =
                unique_resource<impl::vbo<TTarget>, gl_vbo_deleter>;
        }

        template <GLenum TTarget>
        auto make_vbo(GLuint n)
        {
            impl::vbo<TTarget> v{n};
            v.generate();

            return impl::unique_vbo<TTarget>{v};
        }
    }
}