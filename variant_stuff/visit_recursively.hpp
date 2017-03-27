// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./overload.hpp"
#include "./probably_callable.hpp"
#include "./utils.hpp"
#include "./visit.hpp"
#include "./visit_helper.hpp"

/*
#include <iostream>
#include <typeinfo>
*/

namespace vr::impl
{
    template <typename TF>
    auto adapt(TF&& f)
    {
        return [f = FWD(f)](auto, auto&& x)->decltype(f(FWD(x)))
        {
            return f(FWD(x));
        };
    }

    template <typename... TFs>
    struct rv_unary_functions : TFs...
    {
        template <typename... TFFwds>
        rv_unary_functions(TFFwds&&... fs) : TFs(FWD(fs))...
        {
        }

        template <typename... TXs>
        auto operator()(TXs&&... xs)
        {
            auto o = overload(adapt(static_cast<TFs&>(*this))..., FWD(xs)...);
            return [o = std::move(o)](auto&&... variants) mutable->decltype(
                auto)
            {
                return vr::visit([&o](auto&&... xs) -> decltype(
                                     auto) { return o(0, FWD(xs)...); },
                    FWD(variants)...);
            };
        }
    };
}

namespace vr
{
    template <typename... TFs>
    auto make_recursive_visitor(TFs&&... fs)
    {
        return impl::rv_unary_functions<std::decay_t<TFs>...>(FWD(fs)...);
    }

    template <typename... TFs>
    auto visit_recursively(TFs&&... fs)
    {
        return make_recursive_visitor(FWD(fs)...);
    }
}

/*
namespace vr::impl
{
    template <typename...>
    struct concat_alternatives;

    template <template <typename...> class TV0, typename... T0s>
    struct concat_alternatives<TV0<T0s...>>
    {
        using type = std::tuple<T0s...>;
    };

    template <template <typename...> class TV0, typename... T0s,
        template <typename...> class TV1, typename... T1s, typename... TRest>
    struct concat_alternatives<TV0<T0s...>, TV1<T1s...>, TRest...>
    {
        using type = typename concat_alternatives<std::tuple<T0s..., T1s...>,
            TRest...>::type;
    };

    template <template <typename...> class TTemplate, typename, typename>
    struct expand;

    template <template <typename...> class TTemplate, typename T,
        template <typename...> class TExpanded, typename... Ts>
    struct expand<TTemplate, T, TExpanded<Ts...>>
    {
        using any = TTemplate<T, Ts...>;
    };

    template <typename... TVariants>
    struct adaptor
    {
        using all_alternatives =
            typename concat_alternatives<TVariants...>::type;

        template <typename TF>
        static auto adapt(TF&& f)
        {
            // clang-format off
            if constexpr(typename expand<is_probably_unary_callable, TF&&,
                all_alternatives>::any{})
            {
                std::cout << "unary\n";
                return f;
            }
            else if constexpr(typename expand<is_probably_binary_callable, TF&&,
                    all_alternatives>::any{})
            {
                std::cout << "binary\n";
                return f;
            }
            else
            {
                std::cout << "undeducible\n";
                return f;
            }
            // clang-format on

            /*
            std::apply(
                [](auto&&... xs) {
                    (std::cout << typeid(std::decay_t<decltype(xs)>).name()
                               << "\n",
                        ...);
                },
                all_alternatives{});

}
}
;

template <typename... TFs>
class recursive_visitor
{
private:
    // TODO: EBO
    std::tuple<TFs...> _fs;

public:
    template <typename... TFFwds>
    constexpr recursive_visitor(TFFwds&&... fs) : _fs(FWD(fs)...)
    {
    }

    template <typename... TVariants>
    constexpr decltype(auto) operator()(TVariants&&... variants)
    {
        // TODO: reference_overload(...)
        auto visitor = std::apply(
            [](auto&... xs) {
                return overload(
                    adaptor<std::decay_t<TVariants>...>::adapt(xs)...);
            },
            _fs);
    }
};

template <typename... TFs>
constexpr auto make_recursive_visitor(TFs&&... fs)
{
    return recursive_visitor<std::decay_t<TFs>...>(FWD(fs)...);
}
}

namespace vr
{
    template <typename... TFs>
    constexpr auto visit_recursively(TFs&&... fs)
    {
        return impl::make_recursive_visitor(FWD(fs)...);
    }
}
*/