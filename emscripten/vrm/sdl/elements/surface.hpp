// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        class surface : public impl::sdl_element<SDL_Surface>
        {
        private:
            SDL_Rect _rect;

        public:
            using base_type = impl::sdl_element<SDL_Surface>;

            auto width() const noexcept { return get().w; }
            auto height() const noexcept { return get().h; }
            auto size() const noexcept { return make_vec2(width(), height()); }
            const auto& rect() const noexcept { return _rect; }

            surface(const std::string& path) noexcept
                : base_type{IMG_Load(path.c_str())}
            {
                _rect.x = 0;
                _rect.y = 0;
                _rect.w = width();
                _rect.h = height();
            }

            surface(SDL_Surface* p) noexcept : base_type{p} {}



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

            void blit(const surface& s, const SDL_Rect& src_rect,
                const SDL_Rect& dest_rect)
            {
                SDL_BlitSurface(s.ptr(), &src_rect, ptr(),
                    &const_cast<SDL_Rect&>(dest_rect));
            }

            void blit(const surface& i, int x, int y);
        };
    }
}