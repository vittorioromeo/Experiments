// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./legacy.hpp"

template <typename>
using void_t = void;

template <typename T>
using is_zero_sized = std::integral_constant<bool, sizeof(T) == 1>;

template <typename T, typename = void>
struct has_init : std::false_type
{
};

template <typename T>
struct has_init<T, void_t<decltype(&T::init)>> : std::true_type
{
};

template <typename T>
using is_valid_behavior = std::integral_constant<bool, // .
    is_zero_sized<T>{} && has_init<T>{}                // .
    >;

// TODO: check other methods


namespace behavior
{
    template <typename T>
    struct free_store_b
    {
        using handle_type = T*;

        free_store_b() = delete;

        static handle_type null_handle()
        {
            return nullptr;
        }

        static handle_type init(T* ptr)
        {
            return legacy::free_store_new<T>(ptr);
        }

        static void deinit(const handle_type& handle)
        {
            legacy::free_store_delete(handle);
        }
    };
    static_assert(is_valid_behavior<free_store_b<int>>{}, "");

    struct vbo_b
    {
        struct vbo_handle
        {
            legacy::GLuint _id;
            legacy::GLsizei _n;

            auto operator==(const vbo_handle& rhs) const noexcept
            {
                return _id == rhs._id && _n == rhs._n;
            }

            auto operator!=(const vbo_handle& rhs) const noexcept
            {
                return !(*this == rhs);
            }
        };

        using handle_type = vbo_handle;

        vbo_b() = delete;

        static handle_type null_handle()
        {
            return {0, 0};
        }

        static handle_type init(std::size_t n)
        {
            handle_type result;

            legacy::glGenBuffers(n, &result._id);
            result._n = n;

            return result;
        }

        static void deinit(const handle_type& handle)
        {
            legacy::glDeleteBuffers(handle._n, &handle._id);
        }
    };
    static_assert(is_valid_behavior<vbo_b>{}, "");

    struct file_b
    {
        using handle_type = int;

        file_b() = delete;

        static handle_type null_handle()
        {
            return -1;
        }

        static handle_type init()
        {
            return legacy::open_file();
        }

        static void deinit(const handle_type& handle)
        {
            legacy::close_file(handle);
        }
    };
    static_assert(is_valid_behavior<file_b>{}, "");
}