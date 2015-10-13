// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/check.hpp>

namespace vrm
{
    namespace sdl
    {
        enum class buffer_target
        {
            array,
            element_array
        };

        namespace impl
        {
            template <buffer_target TV>
            constexpr GLenum buffer_target_value{GL_ARRAY_BUFFER};

            template <>
            constexpr GLenum buffer_target_value<buffer_target::element_array>{
                GL_ELEMENT_ARRAY_BUFFER};
        }

        enum class buffer_usage
        {
            stream_draw,
            static_draw,
            dynamic_draw
        };

        namespace impl
        {
            template <buffer_usage TV>
            constexpr GLenum buffer_usage_value{GL_STREAM_DRAW};

            template <>
            constexpr GLenum buffer_usage_value<buffer_usage::static_draw>{
                GL_STATIC_DRAW};

            template <>
            constexpr GLenum buffer_usage_value<buffer_usage::dynamic_draw>{
                GL_DYNAMIC_DRAW};
        }


        namespace impl
        {
            template <buffer_target TTarget>
            struct vbo
            {
                static constexpr GLenum target_value{
                    impl::buffer_target_value<TTarget>};

                GLuint _id, _n;

                vbo() = default;
                vbo(GLuint n) noexcept : _n{n} {}

                void generate() noexcept
                {
                    VRM_SDL_GLCHECK(glGenBuffers(_n, &_id));
                }
                void deleteVBO() noexcept
                {
                    VRM_SDL_GLCHECK(glDeleteBuffers(_n, &_id));
                }
                void bind() noexcept
                {
                    // std::cout << "bound vbo " << _id << "\n";
                    VRM_SDL_GLCHECK(glBindBuffer(target_value, _id));
                }
                void unbind() noexcept
                {
                    VRM_SDL_GLCHECK(glBindBuffer(target_value, 0));
                }

                template <typename... Ts>
                void sub_buffer_data(Ts&&... xs)
                {
                    VRM_SDL_GLCHECK(glBufferSubData(FWD(xs)...));
                }

                template <buffer_usage TUsage, typename T>
                void buffer_data(T* data, sz_t count) noexcept
                {
                    /*std::cout << "buffer:\n"
                              << "\tcount * sizeof(T) = " << count * sizeof(T)
                              << "\n"
                              << "\tdata = " << (int)(data) << "\n";*/

                    VRM_SDL_GLCHECK(
                        glBufferData(target_value, count * sizeof(T), data,
                            impl::buffer_usage_value<TUsage>));
                }

                template <buffer_usage TUsage, typename T, sz_t TN>
                void buffer_data(T(&arr)[TN]) noexcept
                {
                    buffer_data<TUsage>(&arr[0], TN);
                }

                template <buffer_usage TUsage, typename T>
                void buffer_data(const std::vector<T>& vec, sz_t count) noexcept
                {
                    buffer_data<TUsage>(vec.data(), count);
                }

                template <buffer_usage TUsage, typename T>
                void buffer_data(const std::vector<T>& vec, sz_t count,
                    sz_t item_offset) noexcept
                {
                    buffer_data<TUsage>(
                        vec.data() + (sizeof(T) * item_offset), count);
                }

                template <buffer_usage TUsage, typename T>
                void buffer_data(const std::vector<T>& vec) noexcept
                {
                    buffer_data<TUsage>(vec, vec.size());
                }

                template <typename TF>
                void with(TF&& f) noexcept
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

            template <buffer_target TTarget>
            using unique_vbo =
                unique_resource<impl::vbo<TTarget>, gl_vbo_deleter>;
        }

        template <buffer_target TTarget>
        auto make_vbo(GLuint n) noexcept
        {
            impl::vbo<TTarget> v{n};
            v.generate();

            return impl::unique_vbo<TTarget>{v};
        }
    }
}