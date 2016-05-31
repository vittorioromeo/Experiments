#pragma once

#include "./dependencies.hpp"

/// @brief Round up `x` to the nearest multiple of `multiple`.
template <typename T0, typename T1>
constexpr auto multiple_round_up(T0 x, T1 multiple) noexcept
{
    VRM_CORE_CONSTEXPR_ASSERT(multiple != 0);
    return ((x + multiple - 1) / multiple) * multiple;
}