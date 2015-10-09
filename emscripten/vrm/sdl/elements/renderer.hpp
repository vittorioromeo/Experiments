// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        class renderer : public impl::unique_renderer
        {
        private:
            using base_type = impl::unique_renderer;

        public:
            renderer(window& w) noexcept
                : base_type{SDL_CreateRenderer(w, -1,
                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE |
                          SDL_RENDERER_PRESENTVSYNC)}
            {
            }

            void draw_color(int r, int g, int b, int a) noexcept
            {
                SDL_SetRenderDrawColor(*this, r, g, b, a);
            }

            void clear() noexcept { SDL_RenderClear(*this); }
            void clear(int r, int g, int b, int a) noexcept
            {
                draw_color(r, g, b, a);
                clear();
            }

            void blend_mode(SDL_BlendMode m) noexcept
            {
                SDL_SetRenderDrawBlendMode(*this, m);
            }

            void present() noexcept { SDL_RenderPresent(*this); }

            void target(std::nullptr_t) noexcept
            {
                SDL_SetRenderTarget(*this, nullptr);
            }
            void target(texture& t) noexcept;

            void clear_texture(texture& t, int r, int g, int b, int a) noexcept;
            void draw(texture& t) noexcept;
            void draw(texture& t, const vec2f& pos) noexcept;

            void draw(sprite& s) noexcept;

            void load_texture(texture& t, const std::string& path);
        };
    }
}