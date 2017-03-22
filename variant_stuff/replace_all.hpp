// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./utils.hpp"

namespace vr::impl
{
    template <typename TBefore, typename TAfter, typename T>
    struct replace_all_helper
    {
        using type = std::conditional_t<std::is_same_v<T, TBefore>, TAfter, T>;
    };

    template <typename TBefore, typename TAfter,
        template <typename...> class TTemplate, typename... Ts>
    struct replace_all_helper<TBefore, TAfter, TTemplate<Ts...>>
    {
        using type = TTemplate<
            typename replace_all_helper<TBefore, TAfter, Ts>::type...>;
    };
}

namespace vr
{
    template <typename TBefore, typename TAfter, typename T>
    using replace_all =
        typename impl::replace_all_helper<TBefore, TAfter, T>::type;
}