// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        class sprite
        {
        private:
            texture* _texture{nullptr};
            impl::space_data _sd;

        public:
            sprite() = default;
            sprite(texture& t) noexcept : _texture{&t} {}

            sprite(const sprite& s) = default;
            sprite& operator=(const sprite& s) = default;

            sprite(sprite&& s) = default;
            sprite& operator=(sprite&& s) = default;

            void texture(texture& t) noexcept { _texture = &t; }
            auto valid_texture() const noexcept { return _texture != nullptr; }

            auto& texture() noexcept { return *_texture; }
            const auto& texture() const noexcept { return *_texture; }

            auto& pos() noexcept { return _sd.pos(); }
            const auto& pos() const noexcept { return _sd.pos(); }

            auto& scale() noexcept { return _sd.scale(); }
            const auto& scale() const noexcept { return _sd.scale(); }

            auto& origin() noexcept { return _sd.origin(); }
            const auto& origin() const noexcept { return _sd.origin(); }

            auto& radians() noexcept { return _sd.radians(); }
            const auto& radians() const noexcept { return _sd.radians(); }

            void set_origin_to_center() noexcept
            {
                origin() = texture().size() / 2.f;
            }
        };
    }
}