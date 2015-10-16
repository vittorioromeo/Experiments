// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <chrono>
#include <vrm/sdl/math.hpp>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/utils.hpp>
#include <vrm/sdl/resource.hpp>
#include <vrm/sdl/elements.hpp>
#include <vrm/sdl/context/unique_sdl_resources.hpp>
#include <vrm/sdl/context/context_settings.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        class static_timer
        {
        public:
            ft _step, _time_slice, _time{0};
            float _interp_t{0};
            sz_t _max_loops, _loops{0};

        public:
            static_timer(ft step = 1.f, ft time_slice = 16.f,
                sz_t max_loops = 50) noexcept : _step{step},
                                                _time_slice{time_slice},
                                                _max_loops{max_loops}
            {
            }

            template <typename TF>
            void run(ft frame_time, TF&& f) noexcept(noexcept(f(_step)))
            {
                _loops = 0;
                _time += frame_time;

                while(_time >= _time_slice && _loops < _max_loops) {
                    f(_step);
                    _time -= _time_slice;
                    ++_loops;
                }

                _interp_t = _time / _time_slice;
            }

            const auto& interp_t() const noexcept { return _interp_t; }
            const auto& time_slice() const noexcept { return _time_slice; }
        };
    }
}
VRM_SDL_NAMESPACE_END