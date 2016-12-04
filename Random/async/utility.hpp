#pragma once

#include <cassert>
#include <cstdio>
#include <vrm/core/type_traits.hpp>
#include <vrm/core/for_args.hpp>
#include "threading.hpp"

#define IF_CONSTEXPR \
    if               \
    constexpr

namespace orizzonte
{
    using vrm::core::forward_like;
    using vrm::core::copy_if_rvalue;
    using vrm::core::for_args;
    using vrm::core::for_args_data;
    using vrm::core::int_v;

    inline void sleep_ms(int ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    template <typename T>
    inline void print_sleep_ms(int ms, const T& x)
    {
        std::puts(x);
        sleep_ms(ms);
    }
}

namespace ll = orizzonte;