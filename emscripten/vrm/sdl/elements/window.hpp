// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        class window
            : public impl::sdl_element<SDL_Window>
        {
        public:
            using base_type =
                impl::sdl_element<SDL_Window>;

            window(sz_t width, sz_t height) noexcept
                : base_type{SDL_CreateWindow("emscripten window", 0, 0, width,
                      height, SDL_WINDOW_OPENGL)}
            {
            }
        };
    }
}