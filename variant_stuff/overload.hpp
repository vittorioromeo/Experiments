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
        return overloader<Ts...>{FWD(xs)...};
    }
}
