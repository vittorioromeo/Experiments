// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/math.hpp>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/elements/sdl_element.hpp>

namespace vrm
{
    namespace sdl
    {
        class renderer : public impl::sdl_element<SDL_Renderer>
        {
        public:
            using base_type =
                impl::sdl_element<SDL_Renderer>;
            renderer(window& w) noexcept;

            void draw_color(int r, int g, int b, int a) noexcept;

            void clear() noexcept;
            void clear(int r, int g, int b, int a) noexcept;

            void blend_mode(SDL_BlendMode m) noexcept;

            void present() noexcept;

            void target(std::nullptr_t) noexcept;
            void target(texture& t) noexcept;

            void clear_texture(texture& t, int r, int g, int b, int a) noexcept;

            void draw(texture& t) noexcept;
            void draw(texture& t, const vec2f& pos) noexcept;
            void draw(sprite& s) noexcept;
        };
    }
}