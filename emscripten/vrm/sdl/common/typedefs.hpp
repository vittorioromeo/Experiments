// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cstddef>
#include <functional>
#include <chrono>
#include <vrm/sdl/common/dependencies.hpp>

namespace vrm
{
    namespace sdl
    {
        using key_code = SDL_Keycode;
        using mouse_coord = Sint32;
        using mouse_btn = Uint8;
        using sz_t = std::size_t;

        using hr_clock = std::chrono::high_resolution_clock;
        using hr_duration = typename hr_clock::duration;

        using ft = float;

        using update_fn = std::function<void(ft)>;
        using draw_fn = std::function<void()>;

        class context;
        class window;
        class renderer;
        class texture;
        class surface;
        class ttffont;
        class sprite;
    }
}