// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            class input_state
            {
                friend class ::vrm::sdl::context;

            private:
                std::unordered_map<key_code, bool> _keys;
                std::unordered_map<mouse_btn, bool> _btns;
                mouse_coord _mouse_x{0};
                mouse_coord _mouse_y{0};

                auto& key(key_code c) { return _keys[c]; }
                auto& btn(mouse_btn b) { return _btns[b]; }

                void mouse_x(mouse_coord c) noexcept { _mouse_x = c; }
                void mouse_y(mouse_coord c) noexcept { _mouse_y = c; }

            public:
                auto key(key_code c) const { return _keys.at(c); }
                auto btn(mouse_btn b) const { return _btns.at(b); }

                auto mouse_x() const noexcept { return _mouse_x; }
                auto mouse_y() const noexcept { return _mouse_y; }
                auto mouse_pos() const noexcept
                {
                    return make_vec2(mouse_x(), mouse_y());
                }
            };
        }
    }
}