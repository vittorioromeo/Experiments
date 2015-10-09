// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        void renderer::load_texture(texture& t, const std::string& path)
        {
            image temp{path};
            t = texture{*this, temp};
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

            SDL_Rect dst;
            dst.x = s.pos().x() - s.origin().x();
            dst.y = s.pos().y() - s.origin().y();
            dst.w = s.texture().size().x();
            dst.h = s.texture().size().y();

            SDL_Point center{(int)s.origin().x(), (int)s.origin().y()};

            SDL_RenderCopyEx(*this, s.texture(), nullptr, &dst,
                to_deg(s.radians()), &center, SDL_FLIP_NONE);


            // SDL_RenderCopy(*this, t, nullptr, &dst);

            // draw(s.texture(), s.pos() - s.origin());
        }
    }
}