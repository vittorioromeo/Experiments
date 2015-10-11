// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/elements/sdl_element.hpp>
#include <vrm/sdl/elements/space_data.hpp>

namespace vrm
{
    namespace sdl
    {
        class texture;

        class sprite
        {
        private:
            texture* _texture{nullptr};
            impl::space_data _sd;

        public:
            sprite() = default;
            sprite(texture& t) noexcept;

            sprite(const sprite& s) = default;
            sprite& operator=(const sprite& s) = default;

            sprite(sprite&& s) = default;
            sprite& operator=(sprite&& s) = default;

            void texture(texture& t) noexcept;
            auto valid_texture() const noexcept;

            auto& texture() noexcept;
            const auto& texture() const noexcept;

            auto& pos() noexcept;
            const auto& pos() const noexcept;

            auto& scale() noexcept;
            const auto& scale() const noexcept;

            auto& origin() noexcept;
            const auto& origin() const noexcept;

            auto& radians() noexcept;
            const auto& radians() const noexcept;

            void set_origin_to_center() noexcept;
        };
    }
}
