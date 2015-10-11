// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <utility>

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
                T _res;
                TDeleter _deleter;
                bool _must_delete;

            public:
                void reset() noexcept
                {
                    if(!_must_delete) return;

                    _deleter(_res);
                    _must_delete = false;
                }

                void reset(T&& r) noexcept
                {
                    reset();
                    _res = std::move(r);
                    _must_delete = true;
                }

                void release() noexcept { _must_delete = false; }

                template <typename... Ts>
                unique_resource(Ts&&... xs) noexcept(noexcept(T(FWD(xs)...)))
                    : _res(FWD(xs)...), _must_delete{true}
                {
                }

                unique_resource(const unique_resource&) = delete;
                unique_resource& operator=(const unique_resource&) = delete;

                unique_resource(unique_resource&& s) noexcept
                    : _res{std::move(s._res)},
                      _deleter{std::move(s._deleter)},
                      _must_delete{s._must_delete}
                {
                    // reset();
                    s.release();
                }
                unique_resource& operator=(unique_resource&& s) noexcept
                {
                    reset();

                    _res = std::move(s._res);
                    _deleter = std::move(s._deleter);
                    _must_delete = s._must_delete;

                    s.release();
                    return *this;
                }

                ~unique_resource() noexcept { reset(); }

                auto operator-> () noexcept { return &_res; }
                auto operator-> () const noexcept { return &_res; }

                auto& operator*() noexcept { return _res; }
                const auto& operator*() const noexcept { return _res; }
            };
        }
    }
}