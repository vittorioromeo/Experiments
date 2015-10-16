// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common/typedefs.hpp>

VRM_SDL_NAMESPACE
{

    enum class mbtn : sz_t
    {
        left = 1,
        middle = 2,
        right = 3,
        wheelup = 4,
        wheeldown = 5,
        x1 = 6,
        x2 = 7
    };

    namespace impl
    {
        constexpr sz_t mbtn_count{8};
        using btn_event_handler = std::function<void(mbtn)>;
    }
}
VRM_SDL_NAMESPACE_END
