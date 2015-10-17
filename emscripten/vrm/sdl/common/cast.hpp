// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/dependencies.hpp>
#include <vrm/sdl/common/macros.hpp>
#include <vrm/sdl/common/typedefs.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        template <typename TBase, typename T>
        using is_same_or_base_of = std::integral_constant<bool,
            std::is_same<TBase, T>{} || std::is_base_of<TBase, T>{}>;
    }

    template <typename T, typename TBase>
    VRM_SDL_ALWAYS_INLINE constexpr T& cast_up(TBase & base) noexcept
    {
        static_assert(impl::is_same_or_base_of<TBase, T>{}, "");
        return static_cast<T&>(base);
    }

    template <typename T, typename TBase>
    VRM_SDL_ALWAYS_INLINE constexpr T* cast_up(TBase * base) noexcept
    {
        static_assert(impl::is_same_or_base_of<TBase, T>{}, "");
        return static_cast<T*>(base);
    }

    template <typename T, typename TBase>
    VRM_SDL_ALWAYS_INLINE constexpr const T& cast_up(const TBase& base) noexcept
    {
        static_assert(impl::is_same_or_base_of<TBase, T>{}, "");
        return static_cast<const T&>(base);
    }

    template <typename T, typename TBase>
    VRM_SDL_ALWAYS_INLINE constexpr const T* cast_up(const TBase* base) noexcept
    {
        static_assert(impl::is_same_or_base_of<TBase, T>{}, "");
        return static_cast<const T*>(base);
    }

    template <typename T>
    VRM_SDL_ALWAYS_INLINE constexpr auto to_void_ptr(T * x) noexcept
    {
        return static_cast<void*>(x);
    }

    template <typename T>
    VRM_SDL_ALWAYS_INLINE constexpr auto to_void_ptr(const T* x) noexcept
    {
        return static_cast<const void*>(x);
    }

    template <typename T>
    VRM_SDL_ALWAYS_INLINE constexpr auto to_void_ptr(
        T & x) noexcept->std::enable_if_t<!std::is_pointer<T>{}, void*>
    {
        return reinterpret_cast<void*>(x);
    }

    template <typename T>
    VRM_SDL_ALWAYS_INLINE constexpr auto to_void_ptr(const T& x) noexcept
        ->std::enable_if_t<!std::is_pointer<T>{}, const void*>
    {
        return reinterpret_cast<const void*>(x);
    }

    template <typename T>
    VRM_SDL_ALWAYS_INLINE constexpr auto from_enum(const T& x) noexcept
    {
        return static_cast<std::underlying_type_t<T>>(x);
    }


    template <typename TOut, typename TIn>
    VRM_SDL_ALWAYS_INLINE constexpr auto to_num(const TIn& x) noexcept
        ->std::enable_if_t<std::is_arithmetic<TOut>{} &&
                               std::is_arithmetic<TIn>() &&
                               !std::is_enum<TOut>() && !std::is_enum<TIn>(),
            TOut>
    {
        return static_cast<TOut>(x);
    }

    template <typename TOut, typename TIn>
    VRM_SDL_ALWAYS_INLINE constexpr auto to_enum(const TIn& x) noexcept
        ->std::enable_if_t<
            std::is_enum<TOut>() && !std::is_enum<TIn>() &&
                std::is_convertible<std::underlying_type_t<TOut>, TIn>{},
            TOut>
    {
        return static_cast<TOut>(x);
    }

    template <typename TOut, typename TIn>
    VRM_SDL_ALWAYS_INLINE constexpr auto to_enum(const TIn& x) noexcept
        ->std::enable_if_t<std::is_enum<TOut>() && std::is_enum<TIn>(), TOut>
    {
        static_assert(std::is_convertible<std::underlying_type_t<TOut>,
                          std::underlying_type_t<TIn>>{},
            "");

        return static_cast<TOut>(x);
    }
}
VRM_SDL_NAMESPACE_END