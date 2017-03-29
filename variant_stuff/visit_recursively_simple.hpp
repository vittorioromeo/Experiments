// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./overload.hpp"
#include "./utils.hpp"
#include "./visit_helper.hpp"
#include <boost/hana/functional/fix.hpp>

namespace vr
{
    template <typename TVisitor>
    constexpr decltype(auto) visit_recursively_simple(TVisitor&& visitor)
    {
        return FWD(visitor)();
    }

    template <typename TVisitor, typename TVariant, typename... TVariants>
    constexpr decltype(auto) visit_recursively_simple(
        TVisitor&& visitor, TVariant&& variant, TVariants&&... variants)
    {
        using variant_type = std::decay_t<decltype(variant._data)>;
        using helper = typename visit_homogenizer_helper<variant_type>::type;

        return helper{}(FWD(visitor), FWD(variant)._data, FWD(variants)._data...);
    }

    template <typename TReturn, typename... TFs>
    constexpr auto make_recursive_visitor(TFs&&... fs)
    {
        return boost::hana::fix([&fs...](auto self, auto&&... xs) -> TReturn {
            return overload(std::forward<TFs>(fs)...)(
                [&self](auto&&... vs) {
                    return visit_recursively_simple(self, FWD(vs)...);
                },
                FWD(xs)...);
        });
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
