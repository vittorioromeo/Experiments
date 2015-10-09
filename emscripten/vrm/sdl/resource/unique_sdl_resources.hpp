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
            struct window_deleter
            {
                void operator()(SDL_Window* p) noexcept
                {
                    SDL_DestroyWindow(p);
                }
            };

            struct texture_deleter
            {
                void operator()(SDL_Texture* p) noexcept
                {
                    SDL_DestroyTexture(p);
                }
            };

            struct renderer_deleter
            {
                void operator()(SDL_Renderer* p) noexcept
                {
                    SDL_DestroyRenderer(p);
                }
            };

            struct surface_deleter
            {
                void operator()(SDL_Surface* p) noexcept { SDL_FreeSurface(p); }
            };

            struct glcontext_deleter
            {
                void operator()(SDL_GLContext* p) noexcept
                {
                    SDL_GL_DeleteContext(p);
                }
            };

            struct ttffont_deleter
            {
                void operator()(TTF_Font* p) noexcept { TTF_CloseFont(p); }
            };
        }
    }
}