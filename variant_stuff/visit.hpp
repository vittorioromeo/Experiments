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

    template <typename... TFs>
    constexpr decltype(auto) visit_in_place(TFs&&... fs)
    {
        return [visitor = overload(FWD(fs)...)](auto&&... variants) mutable
            ->decltype(auto)
        {
            return visit(visitor, FWD(variants)...);
        };
    }
}