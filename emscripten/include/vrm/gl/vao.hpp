// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/gl/common.hpp>
#include <vrm/gl/check.hpp>
#include <vrm/gl/primitive.hpp>
#include <vrm/gl/index_type.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        class vao
        {
        private:
            GLuint _id, _n;

            bool bound() const noexcept
            {
#ifdef GL_VERTEX_ARRAY_BINDING
                GLint result;

                VRM_SDL_GLCHECK(
                    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &result));

                return result == vrmc::to_num<GLint>(_id);
#else
                return true;
#endif
            }


        public:
            vao() = default;
            vao(GLuint n) noexcept : _n{n} {}

            void generate() noexcept
            {
                VRM_CORE_ASSERT(_n == 1);
                VRM_SDL_GLCHECK(glGenVertexArraysOES(_n, &_id));
            }

            void deleteVAO() noexcept
            {
                VRM_SDL_GLCHECK(glDeleteVertexArraysOES(_n, &_id));
            }

            void bind() noexcept { VRM_SDL_GLCHECK(glBindVertexArrayOES(_id)); }

            void unbind() noexcept
            {
                VRM_CORE_ASSERT(bound());
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
            void draw_arrays(GLint first_index, GLsizei index_count) noexcept
            {
                VRM_CORE_ASSERT(bound());

                VRM_SDL_GLCHECK(glDrawArrays(
                    vrmc::from_enum(TP), first_index, index_count));
            }

            template <primitive TP, index_type TI>
            void draw_elements(
                GLsizei index_count, sz_t vbo_offset_byte = 0) noexcept
            {
                VRM_CORE_ASSERT(bound());

                VRM_SDL_GLCHECK(glDrawElements(vrmc::from_enum(TP), index_count,
                    vrmc::from_enum(TI),
                    vrmc::num_to_void_ptr(vbo_offset_byte)));
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

    auto make_vao(GLuint n = 1) noexcept
    {
        VRM_CORE_ASSERT(n > 0);

        impl::vao v{n};
        v.generate();

        return impl::unique_vao{v};
    }
}
VRM_SDL_NAMESPACE_END