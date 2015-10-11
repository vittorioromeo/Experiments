// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <iostream>
#include <vrm/sdl/dependencies.hpp>

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            template <typename T>
            auto error_title_for() noexcept
            {
                return "";
            }

            template <>
            auto error_title_for<SDL_Renderer>() noexcept
            {
                return "renderer";
            }

            template <>
            auto error_title_for<SDL_Window>() noexcept
            {
                return "window";
            }

            template <>
            auto error_title_for<SDL_Texture>() noexcept
            {
                return "texture";
            }

            template <>
            auto error_title_for<SDL_Surface>() noexcept
            {
                return "surface";
            }


            template <>
            auto error_title_for<SDL_GLContext>() noexcept
            {
                return "glcontext";
            }

            template <>
            auto error_title_for<TTF_Font>() noexcept
            {
                return "ttffont";
            };

            void log_sdl_error(const char* title = "unknown") noexcept
            {
                std::cout << "[" << title << "] "
                          << "sdl error: " << SDL_GetError() << std::endl;
            }
        }
    }
}