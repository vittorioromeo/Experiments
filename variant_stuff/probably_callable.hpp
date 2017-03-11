// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./any_type.hpp"
#include "./callable_utils.hpp"
#include "./deduced_arity.hpp"
#include <experimental/type_traits>
#include <utility>

namespace vr
{
    // Evaluates to `true` if `TF` is callable with `any_type` or any of
    // `THelpers...`.
    template <typename TF, typename... THelpers>
    using is_probably_unary_callable =
        is_unary_callable_with_any<TF, any_type, THelpers...>;

    // Evaluates to `true` if `TF` is callable with any binary combination of
    // `any_type` or any of `THelpers...`.
    template <typename TF, typename... THelpers>
    using is_probably_binary_callable =
        is_binary_callable_with_any<TF, any_type, THelpers...>;
}