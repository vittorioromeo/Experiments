// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./utils.hpp"

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
}
