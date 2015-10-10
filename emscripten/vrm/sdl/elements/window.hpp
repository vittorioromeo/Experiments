// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        class window : public impl::sdl_element<SDL_Window>
        {
        public:
            using base_type = impl::sdl_element<SDL_Window>;

            window(const std::string& title, sz_t width, sz_t height) noexcept
                : base_type{SDL_CreateWindow(title.c_str(),
                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width,
                      height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)}
            {
            }

            void title(const std::string& s) noexcept
            {
                SDL_SetWindowTitle(*this, s.c_str());
            }
        };
    }
}