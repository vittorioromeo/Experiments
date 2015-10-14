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

                void sub_buffer_data(const void* data_ptr,
                    GLsizeiptr byte_count,
                    GLintptr vbo_byte_offset = 0) noexcept
                {
                    assert(byte_count >= 0);
                    assert(vbo_byte_offset >= 0);
                    assert(data_ptr != nullptr);

                    /*
                    std::cout << "glBufferSubData("
                              << "target_value=" << target_value
                              << ", vbo_byte_offset=" << vbo_byte_offset
                              << ", byte_count=" << byte_count
                              << ", data_ptr)\n";
                    */

                    VRM_SDL_GLCHECK(glBufferSubData(
                        target_value, vbo_byte_offset, byte_count, data_ptr));
                }

                void sub_buffer_data_bytes(const void* data_ptr,
                    sz_t byte_count, sz_t vbo_byte_offset = 0) noexcept
                {
                    sub_buffer_data(data_ptr, byte_count, vbo_byte_offset);
                }

                template <typename T>
                void sub_buffer_data_items(const T* data_ptr, sz_t item_count,
                    sz_t vbo_byte_offset = 0) noexcept
                {
                    sub_buffer_data_bytes(static_cast<const void*>(data_ptr),
                        item_count * sizeof(T), vbo_byte_offset);
                }

                template <typename T>
                void sub_buffer_data_items(const std::vector<T>& vec,
                    sz_t item_count_offset, sz_t item_count,
                    sz_t vbo_byte_offset = 0) noexcept
                {
                    assert(vec.size() - item_count_offset >= item_count);

                    sub_buffer_data_items<T>(vec.data() + item_count_offset,
                        item_count, vbo_byte_offset);
                }

                template <buffer_usage TUsage>
                void buffer_data(
                    const void* data_ptr, GLsizeiptr byte_count) noexcept
                {
                    VRM_SDL_GLCHECK(glBufferData(target_value, byte_count,
                        data_ptr, impl::buffer_usage_value<TUsage>));
                }

                template <buffer_usage TUsage>
                void buffer_data_bytes(
                    const void* data_ptr, sz_t byte_count) noexcept
                {
                    buffer_data<TUsage>(data_ptr, byte_count);
                }

                template <buffer_usage TUsage, typename T>
                void buffer_data_items(
                    const T* data_ptr, sz_t item_count) noexcept
                {
                    buffer_data_bytes<TUsage>(
                        static_cast<const void*>(data_ptr),
                        sizeof(T) * item_count);
                }

                template <buffer_usage TUsage, typename T, sz_t TN>
                void buffer_data_items(T(&arr)[TN]) noexcept
                {
                    buffer_data_items<TUsage, T>(&arr[0], TN);
                }

                template <buffer_usage TUsage, typename T>
                void buffer_data_items(const std::vector<T>& vec,
                    sz_t item_count, sz_t item_count_offset = 0) noexcept
                {
                    assert(vec.size() - item_count_offset >= item_count);

                    buffer_data_items<TUsage, T>(
                        vec.data() + item_count_offset, item_count);
                }

                template <buffer_usage TUsage, typename T>
                void buffer_data_items(const std::vector<T>& vec) noexcept
                {
                    buffer_data_items<TUsage, T>(vec, vec.size());
                }

                template <buffer_usage TUsage>
                void allocate_buffer_bytes(sz_t byte_count) noexcept
                {
                    buffer_data<TUsage>(nullptr, byte_count);
                }

                template <buffer_usage TUsage, typename T>
                void allocate_buffer_items(sz_t item_count) noexcept
                {
                    allocate_buffer_bytes<TUsage>(sizeof(T) * item_count);
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