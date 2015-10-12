// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/math.hpp>

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            class space_data
            {
            private:
                vec2f _pos, _scale{1.f, 1.f}, _origin;
                float _radians{0.f};

            public:
                space_data() = default;

                space_data(const space_data&) = default;
                space_data& operator=(const space_data&) = default;

                space_data(space_data&&) = default;
                space_data& operator=(space_data&&) = default;

                auto& pos() noexcept { return _pos; }
                const auto& pos() const noexcept { return _pos; }

                auto& scale() noexcept { return _scale; }
                const auto& scale() const noexcept { return _scale; }

                auto& origin() noexcept { return _origin; }
                const auto& origin() const noexcept { return _origin; }

                auto& radians() noexcept { return _radians; }
                const auto& radians() const noexcept { return _radians; }
            };
        }
    }
}