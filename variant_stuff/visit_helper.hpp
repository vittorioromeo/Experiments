// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./utils.hpp"
#include <boost/variant.hpp>
#include <eggs/variant.hpp>
#include <variant>

template <template <typename> class TVariant>
struct visit_homogenizer;

template <typename>
struct visit_homogenizer_helper;

template <template <typename...> class TVariant, typename... Ts>
struct visit_homogenizer_helper<TVariant<Ts...>> final
{
    using type = visit_homogenizer<TVariant>;
};

#define DEFINE_VISIT_HOMOGENIZER(type, function)              \
    template <>                                               \
    struct visit_homogenizer<type> final                      \
    {                                                         \
        template <typename... Ts>                             \
        constexpr decltype(auto) operator()(Ts&&... xs) const \
            noexcept(noexcept(function(FWD(xs)...)))          \
        {                                                     \
            return function(FWD(xs)...);                      \
        }                                                     \
    }

DEFINE_VISIT_HOMOGENIZER(::std::variant, ::std::visit);
DEFINE_VISIT_HOMOGENIZER(::boost::variant, ::boost::apply_visitor);
DEFINE_VISIT_HOMOGENIZER(::eggs::variant, ::eggs::variants::apply);

#undef DEFINE_VISIT_HOMOGENIZER