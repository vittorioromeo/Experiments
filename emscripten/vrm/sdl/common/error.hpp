// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <iostream>
#include <vrm/sdl/dependencies.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        constexpr const char* error_title_for{""};

        template <>
        constexpr const char* error_title_for<SDL_Renderer>{"renderer"};

        template <>
        constexpr const char* error_title_for<SDL_Window>{"window"};

        template <>
        constexpr const char* error_title_for<SDL_Texture>{"texture"};

        template <>
        constexpr const char* error_title_for<SDL_Surface>{"surface"};

        template <>
        constexpr const char* error_title_for<SDL_GLContext>{"glcontext"};

        template <>
        constexpr const char* error_title_for<TTF_Font>{"ttffont"};

        void log_sdl_error(const char* title = "unknown") noexcept
        {
            std::cout << "[" << title << "] "
                      << "sdl error: " << SDL_GetError() << std::endl;
        }
    }
}
VRM_SDL_NAMESPACE_END