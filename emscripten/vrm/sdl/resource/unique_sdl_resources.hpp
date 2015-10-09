// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            struct unique_window_deleter
            {
                void operator()(SDL_Window* p) noexcept
                {
                    SDL_DestroyWindow(p);
                }
            };

            struct unique_texture_deleter
            {
                void operator()(SDL_Texture* p) noexcept
                {
                    SDL_DestroyTexture(p);
                }
            };

            struct unique_renderer_deleter
            {
                void operator()(SDL_Renderer* p) noexcept
                {
                    SDL_DestroyRenderer(p);
                }
            };

            struct unique_surface_deleter
            {
                void operator()(SDL_Surface* p) noexcept { SDL_FreeSurface(p); }
            };

            struct unique_glcontext_deleter
            {
                void operator()(SDL_GLContext* p) noexcept
                {
                    SDL_GL_DeleteContext(p);
                }
            };

            struct unique_ttffont_deleter
            {
                void operator()(TTF_Font* p) noexcept { TTF_CloseFont(p); }
            };

            using unique_window =
                unique_resource<SDL_Window, unique_window_deleter>;

            using unique_texture =
                unique_resource<SDL_Texture, unique_texture_deleter>;

            using unique_renderer =
                unique_resource<SDL_Renderer, unique_renderer_deleter>;

            using unique_surface =
                unique_resource<SDL_Surface, unique_surface_deleter>;

            using unique_glcontext =
                unique_resource<SDL_GLContext, unique_glcontext_deleter>;

            using unique_ttffont =
                unique_resource<TTF_Font, unique_ttffont_deleter>;
        }
    }
}