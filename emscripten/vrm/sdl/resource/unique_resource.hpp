// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            template <typename T, typename TDeleter>
            class unique_resource
            {
            private:
                T* _ptr{nullptr};
                TDeleter deleter;

                void delete_if_necessary() noexcept
                {
                    if(_ptr != nullptr) deleter(_ptr);
                }

            public:
                unique_resource() = default;
                unique_resource(T* p) noexcept { reinit(p); }

                unique_resource(const unique_resource&) = delete;
                unique_resource& operator=(const unique_resource&) = delete;

                unique_resource(unique_resource&& s) noexcept : _ptr{s._ptr}
                {
                    s._ptr = nullptr;
                }
                unique_resource& operator=(unique_resource&& s) noexcept
                {
                    _ptr = s._ptr;
                    s._ptr = nullptr;
                    return *this;
                }

                ~unique_resource() noexcept { delete_if_necessary(); }

                void reinit(T* p) noexcept
                {
                    delete_if_necessary();
                    _ptr = p;

                    if(_ptr == nullptr)
                    {
                        impl::log_sdl_error();
                        std::terminate();
                    }
                }

                auto ptr() noexcept
                {
                    assert(_ptr != nullptr);
                    return _ptr;
                }
                const auto ptr() const noexcept
                {
                    assert(_ptr != nullptr);
                    return _ptr;
                }

                auto& get() noexcept { return *ptr(); }
                const auto& get() const noexcept { return *ptr(); }

                operator T*() noexcept { return ptr(); }
            };
        }
    }
}