// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <experimental/type_traits>
#include <utility>

namespace vr
{
    // Evaluates to `true` if `TF` is callable with any of `Ts...`.
    template <typename TF, typename... Ts>
    using is_unary_callable_with_any =
        std::disjunction<std::is_callable<TF(Ts)>...>;

    namespace impl
    {
        // Checks if `TF` can be called with `T` and any of `Ts...`.
        template <typename TF, typename T>
        class is_binary_callable_with_any_bound final
        {
        private:
            template <typename... Ts>
            using apply_lhs = std::disjunction<std::is_callable<TF(T, Ts)>...>;

            template <typename... Ts>
            using apply_rhs = std::disjunction<std::is_callable<TF(Ts, T)>...>;

        public:
            template <typename... Ts>
            using apply = std::disjunction<apply_lhs<Ts...>, apply_rhs<Ts...>>;
        };
    }

    // Evaluates to `true` if `TF` is binary callable with any of `Ts...`.
    template <typename TF, typename... Ts>
    using is_binary_callable_with_any =
        std::disjunction<typename impl::is_binary_callable_with_any_bound<TF,
            Ts>::template apply<Ts...>...>;
}