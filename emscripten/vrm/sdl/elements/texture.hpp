// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        class texture : public impl::unique_texture
        {
        private:
            using base_type = impl::unique_texture;
            vec2f _size;

        public:
            texture() = default;

            texture(renderer& r, sz_t width, sz_t height) noexcept
                : base_type{SDL_CreateTexture(r, SDL_PIXELFORMAT_ARGB8888,
                      SDL_TEXTUREACCESS_TARGET, width, height)},
                  _size(width, height)
            {
            }

            texture(renderer& r, surface& s) noexcept
                : base_type{SDL_CreateTextureFromSurface(r, s)},
                  _size(s.width(), s.height())
            {
            }

            texture(renderer& r, image& i) noexcept
                : base_type{SDL_CreateTextureFromSurface(r, i.surface())},
                  _size(i.width(), i.height())
            {
            }

            const auto& size() const noexcept { return _size; }
        };
    }
}