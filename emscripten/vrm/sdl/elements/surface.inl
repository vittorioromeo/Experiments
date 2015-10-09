// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        void surface::blit(const image& i, int x, int y)
        {
            blit(i.surface(), i.rect(), SDL_Rect{x, y, i.width(), i.height()});
        }
    }
}