// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        template <typename TF, typename... Ts>
        void for_args(TF&& fn, Ts&&... xs) noexcept(
            noexcept((void)((int[]){(fn(FWD(xs)), 0)...})))
        {
            using swallow = int[];
            return (void)swallow{(fn(FWD(xs)), 0)...};
        }
    }
}
