// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/dependencies.hpp>
#include <vrm/sdl/math.hpp>
#include <vrm/sdl/elements/sdl_element.hpp>
#include <vrm/sdl/elements/surface.hpp>

namespace vrm
{
    namespace sdl
    {
        class texture : public impl::sdl_element<SDL_Texture>
        {
        private:
            vec2f _size;

        public:
            using base_type = impl::sdl_element<SDL_Texture>;

            texture() = default;

            texture(renderer&) noexcept {}

            texture(renderer& r, sz_t width, sz_t height, SDL_TextureAccess access) noexcept
                : base_type{SDL_CreateTexture(r, SDL_PIXELFORMAT_ARGB8888,
                      access, width, height)},
                  _size(width, height)
            {
            }

            texture(renderer& r, surface& s) noexcept
                : base_type{SDL_CreateTextureFromSurface(r, s)},
                  _size(s.width(), s.height())
            {
            }

            const auto& size() const noexcept { return _size; }

            void bind() noexcept
            {
                SDL_GL_BindTexture(*this, nullptr, nullptr);
            }
        };
    }
}