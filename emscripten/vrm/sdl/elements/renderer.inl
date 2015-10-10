// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/elements/renderer.hpp>

namespace vrm
{
    namespace sdl
    {
        renderer::renderer(window& w) noexcept
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

        void renderer::clear_texture(
            texture& t, int r, int g, int b, int a) noexcept
        {
            target(t);
            blend_mode(SDL_BLENDMODE_NONE);
            draw_color(r, g, b, a);
            SDL_RenderFillRect(*this, nullptr);
        }

        void renderer::draw(texture& t) noexcept
        {
            SDL_RenderCopy(*this, t, nullptr, nullptr);
        }

        void renderer::draw(texture& t, const vec2f& pos) noexcept
        {
            SDL_Rect dst;
            dst.x = pos.x();
            dst.y = pos.y();
            dst.w = t.size().x();
            dst.h = t.size().y();

            SDL_RenderCopy(*this, t, nullptr, &dst);
        }

        void renderer::target(texture& t) noexcept
        {
            SDL_SetRenderTarget(*this, t);
        }

        void renderer::draw(sprite& s) noexcept
        {
            assert(s.valid_texture());

            auto scaled_origin(make_vec2(s.origin().x() * s.scale().x(),
                s.origin().y() * s.scale().y()));

            SDL_Rect dst;
            dst.x = s.pos().x() - scaled_origin.x();
            dst.y = s.pos().y() - scaled_origin.y();
            dst.w = s.texture().size().x() * s.scale().x();
            dst.h = s.texture().size().y() * s.scale().y();

            SDL_Point center{(int)scaled_origin.x(), (int)scaled_origin.y()};

            SDL_RenderCopyEx(*this, s.texture(), nullptr, &dst,
                to_deg(s.radians()), &center, SDL_FLIP_NONE);


            // SDL_RenderCopy(*this, t, nullptr, &dst);

            // draw(s.texture(), s.pos() - s.origin());
        }
    }
}