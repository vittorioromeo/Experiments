// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>

VRM_SDL_NAMESPACE
{
    template <typename>
    class delegate;

    struct delegate_handle
    {
        // TODO:
    };

    template <typename TReturn, typename... TArgs>
    class delegate<TReturn(TArgs...)>
    {
    public:
        using fn_type = std::function<TReturn(TArgs...)>;

    private:
        std::vector<fn_type> _functions;

    public:
        template <typename TF>
        auto operator+=(TF&& f)
        {
            _functions.emplace_back(FWD(f));
            return delegate_handle{};
            // TODO:
            // return delegate_handle{_functions.size() - 1};
        }

        auto operator-=(const delegate_handle&)
        {
            // TODO:
            return false;
        }

        template <typename... Ts>
        void operator()(Ts&&... xs) noexcept(noexcept(_functions.back()(xs...)))
        {
            for(auto& f : _functions) f(xs...);
        }

        template <typename TF, typename... Ts>
        void call_and_use_result(TF&& result_handler, Ts&&... xs) noexcept(
            noexcept(result_handler(_functions.back()(xs...))))
        {
            for(auto& f : _functions)
            {
                result_handler(f(xs...));
            }
        }
    };
}
VRM_SDL_NAMESPACE_END
