// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cstddef>
#include <functional>
#include <chrono>
#include <vrm/sdl/dependencies.hpp>

namespace vrm
{
    namespace sdl
    {
        using hr_clock = std::chrono::high_resolution_clock;
        using hr_duration = typename hr_clock::duration;

        using ms_float_duration = std::chrono::duration<float, std::milli>;
        using ms_double_duration = std::chrono::duration<double, std::milli>;

        using sz_t = std::size_t;
        using ft = float;

        namespace impl
        {
            using key_code = SDL_Keycode;
            using mouse_coord = Sint32;
            using mouse_btn = Uint8;
            using update_fn = std::function<void(ft)>;
            using draw_fn = std::function<void()>;
        }

        class context;
        class texture;
        class sprite;

        using namespace std::literals;
    }
}