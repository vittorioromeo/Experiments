// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>

namespace vr
{
    namespace deduced_arity
    {
        namespace impl
        {
            template <int TI>
            struct deducible_t final : std::integral_constant<int, TI>
            {
            };

            struct undeducible_t final
            {
            };
        }

        inline constexpr impl::undeducible_t undeducible{};
        inline constexpr impl::deducible_t<1> unary{};
        inline constexpr impl::deducible_t<2> binary{};
    }
}