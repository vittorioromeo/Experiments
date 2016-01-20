// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/elements/renderer.hpp>

VRM_SDL_NAMESPACE
{
    renderer::renderer(sdl_window & w) noexcept
        : base_type{SDL_CreateRenderer(
              w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE)}
    {
        SDL_RendererInfo sri;
        if(SDL_GetRendererInfo(*this, &sri) == 0)
        {
            printf("Renderer backend: %s\n", sri.name);
        }
        else
        {
            printf("Could not get renderer info\n");
        }
    }

    void renderer::draw_color(int r, int g, int b, int a) noexcept
    {
        SDL_SetRenderDrawColor(*this, r, g, b, a);
    }

    void renderer::clear() noexcept { SDL_RenderClear(*this); }
    void renderer::clear(int r, int g, int b, int a) noexcept
    {
        draw_color(r, g, b, a);
        clear();
    }

    void renderer::blend_mode(SDL_BlendMode m) noexcept
    {
        SDL_SetRenderDrawBlendMode(*this, m);
    }

    void renderer::present() noexcept { SDL_RenderPresent(*this); }

    void renderer::target(std::nullptr_t) noexcept
    {
        SDL_SetRenderTarget(*this, nullptr);
    }
}
VRM_SDL_NAMESPACE_END