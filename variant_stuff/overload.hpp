// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./utils.hpp"
#include <functional>

namespace vr
{
    template <typename... Ts>
    struct overloader : Ts...
    {
        template <typename... TArgs>
        overloader(TArgs&&... xs) : Ts{FWD(xs)}...
        {
        }

        using Ts::operator()...;
    };

    template <typename... Ts>
    auto overload(Ts&&... xs)
    {
        return overloader<std::remove_reference_t<Ts>...>{FWD(xs)...};
    }

    template <typename... Ts>
    auto ref_overload(Ts&... xs)
    {
        return overloader<std::reference_wrapper<Ts>...>{std::ref(xs)...};
    }
}
