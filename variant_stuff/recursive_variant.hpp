// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./replace_all.hpp"
#include "./utils.hpp"
#include <type_traits>

namespace vr::impl
{
    template <typename T, typename TDerived>
    struct recursive_variant_indirection
    {
        T _data;

        template <typename... Ts,
            typename = std::enable_if_t<(sizeof...(Ts) > 1) ||
                                        !std::disjunction_v<std::is_same<
                                            std::decay_t<Ts>, TDerived>...>>>
        recursive_variant_indirection(Ts&&... xs) : _data{FWD(xs)...}
        {
        }
    };

    struct recursive_variant_placeholder
    {
        recursive_variant_placeholder() = delete;
    };

    template <typename TAfter, typename T>
    using replace_placeholders =
        replace_all<recursive_variant_placeholder, TAfter, T>;

    template <typename>
    class recursive_variant_builder;

    template <template <typename...> class TVariant, typename... Ts>
    class recursive_variant_builder<TVariant<Ts...>>
    {
    private:
        struct fwd_variant;
        
        using variant_type_impl =
            replace_placeholders<fwd_variant, TVariant<Ts...>>;

        struct fwd_variant
            : impl::recursive_variant_indirection<variant_type_impl,
                  fwd_variant>
        {
            using impl::recursive_variant_indirection<variant_type_impl,
                fwd_variant>::recursive_variant_indirection;
        };

    public:
        using variant_type = variant_type_impl;

        template <typename T>
        using resolve = replace_placeholders<fwd_variant, T>;
    };
}

namespace vr
{
    using self = impl::recursive_variant_placeholder;
    using impl::recursive_variant_builder;

    template <typename TBuilder>
    using variant_type = typename TBuilder::variant_type;

    template <typename TBuilder, typename T>
    using resolve = typename TBuilder::template resolve<T>;
}