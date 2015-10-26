// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/math.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        class sdl_element
        {
        private:
            T* _ptr{nullptr};

            void check_error() noexcept
            {
#ifndef NDEBUG
                if(_ptr != nullptr) return;

                impl::log_sdl_error(impl::error_title_for<T>);
                std::terminate();
#endif
            }

        public:
            using element_type = T;

            sdl_element() = default;
            sdl_element(T* p) noexcept : _ptr{p} { check_error(); }

            sdl_element(const sdl_element&) = default;
            sdl_element& operator=(const sdl_element&) = default;

            sdl_element(sdl_element&&) = default;
            sdl_element& operator=(sdl_element&&) = default;

            auto ptr() noexcept { return _ptr; }
            auto ptr() const noexcept { return _ptr; }

            auto& get() noexcept
            {
                VRM_CORE_ASSERT(_ptr != nullptr);
                return *ptr();
            }
            const auto& get() const noexcept
            {
                VRM_CORE_ASSERT(_ptr != nullptr);
                return *ptr();
            }

            operator T*() noexcept { return ptr(); }

            auto operator-> () noexcept { return ptr(); }
            auto operator-> () const noexcept { return ptr(); }

            auto& operator*() noexcept { return get(); }
            const auto& operator*() const noexcept { return get(); }
        };
    }
}
VRM_SDL_NAMESPACE_END