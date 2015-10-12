// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/elements/sprite.hpp>
#include <vrm/sdl/elements/texture.hpp>

namespace vrm
{
    namespace sdl
    {
        sprite::sprite(sdl::texture& t) noexcept : _texture{&t} {}

        void sprite::texture(sdl::texture& t) noexcept { _texture = &t; }
        auto sprite::valid_texture() const noexcept
        {
            return _texture != nullptr;
        }

        auto& sprite::texture() noexcept { return *_texture; }
        const auto& sprite::texture() const noexcept { return *_texture; }

        auto& sprite::pos() noexcept { return _sd.pos(); }
        const auto& sprite::pos() const noexcept { return _sd.pos(); }

        auto& sprite::scale() noexcept { return _sd.scale(); }
        const auto& sprite::scale() const noexcept { return _sd.scale(); }

        auto& sprite::origin() noexcept { return _sd.origin(); }
        const auto& sprite::origin() const noexcept { return _sd.origin(); }

        auto& sprite::radians() noexcept { return _sd.radians(); }
        const auto& sprite::radians() const noexcept { return _sd.radians(); }

        void sprite::set_origin_to_center() noexcept
        {
            origin() = texture().size() / 2.f;
        }
    }
}
