// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        class surface : public impl::unique_surface
        {
        private:
            using base_type = impl::unique_surface;

        public:
            using base_type::base_type;

            auto width() const noexcept { return get().w; }
            auto height() const noexcept { return get().h; }
            auto size() const noexcept { return make_vec2(width(), height()); }

            auto format() const noexcept { return get().format; }

            void clear(int r, int g, int b)
            {
                SDL_Rect clear_rect;
                clear_rect.x = 0;
                clear_rect.y = 0;
                clear_rect.w = width();
                clear_rect.h = height();

                SDL_FillRect(ptr(), &clear_rect, SDL_MapRGB(format(), r, g, b));
            }

            void display()
            {
                // SDL_Flip(ptr());
            }

            void blit(const surface& s, const SDL_Rect& src_rect,
                const SDL_Rect& dest_rect)
            {
                SDL_BlitSurface(s.ptr(), &src_rect, ptr(),
                    &const_cast<SDL_Rect&>(dest_rect));
            }

            void blit(const image& i, int x, int y);
        };
    }
}