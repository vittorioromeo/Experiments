// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./overload.hpp"
#include "./utils.hpp"
#include "./visit_helper.hpp"

namespace vr
{
    template <typename TVisitor>
    constexpr decltype(auto) visit(TVisitor&& visitor)
    {
        return FWD(visitor)();
    }

    template <typename TVisitor, typename TVariant, typename... TVariants>
    constexpr decltype(auto) visit(
        TVisitor&& visitor, TVariant&& variant, TVariants&&... variants)
    {
        using variant_type = std::decay_t<TVariant>;
        using helper = typename visit_homogenizer_helper<variant_type>::type;

        return helper{}(FWD(visitor), FWD(variant), FWD(variants)...);
    }

    template <typename... TVariants>
    constexpr decltype(auto) visit_in_place(TVariants&&... variants)
    {
        return [&variants...](auto&&... fs) mutable -> decltype(auto) {
            auto v = overload(FWD(fs)...);

            // Cannot use `FWD(variants)` here because we want to forward with
            // the value category of the original `visit_in_place` arguments,
            // not the of captured ones.
            return visit(v, std::forward<TVariants>(variants)...);
        };
    }
}

// TODO:
// `visit(visitor, variants...)`
// `visit_in_place(functions...)(variants...)`
// `recursively_visit(visitor, variants...)`
// `recursively_visit_in_place(functions...)(variants...)`
// `linearly_visit(visitor, variants...)`
// `linearly_visit_in_place(functions...)(variants...)`
// `recursively_linearly_visit(visitor, variants...)`
// `recursively_linearly_visit_in_place(functions...)(variants...)`
