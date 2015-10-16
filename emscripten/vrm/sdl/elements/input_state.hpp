// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/math.hpp>
#include <vrm/sdl/common.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class context;

        class input_state
        {
            template <typename TSettings>
            friend class ::vrm::sdl::impl::context;

        private:
            std::bitset<impl::kkey_count> _keys;
            std::bitset<impl::mbtn_count> _btns;
            mouse_coord _mouse_x{0};
            mouse_coord _mouse_y{0};

            void key(kkey k, bool x) noexcept
            {
                _keys[static_cast<sz_t>(k)] = x;
            }
            void btn(mbtn b, bool x) noexcept
            {
                _btns[static_cast<sz_t>(b)] = x;
            }

            void mouse_x(mouse_coord c) noexcept { _mouse_x = c; }
            void mouse_y(mouse_coord c) noexcept { _mouse_y = c; }

        public:
            auto key(kkey k) const noexcept
            {
                return _keys[static_cast<sz_t>(k)];
            }
            auto btn(mbtn b) const noexcept
            {
                return _btns[static_cast<sz_t>(b)];
            }

            auto mouse_x() const noexcept { return _mouse_x; }
            auto mouse_y() const noexcept { return _mouse_y; }
            auto mouse_pos() const noexcept
            {
                return make_vec2(mouse_x(), mouse_y());
            }
        };
    }
}
VRM_SDL_NAMESPACE_END