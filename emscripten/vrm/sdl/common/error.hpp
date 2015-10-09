// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <iostream>

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            void log_sdl_error()
            {
                std::cout << "error: " << SDL_GetError() << std::endl;
            }
        }
    }
}