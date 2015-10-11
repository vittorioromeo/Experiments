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

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            class static_timer
            {
                // TODO:
            public:
                ft _step, _time_slice, _time{0};
                float _max_loops, _loops{0};

            public:
                static_timer(ft step = 1.f, ft time_slice = 16.f,
                    float max_loops = 50.f) noexcept : _step{step},
                                                       _time_slice{time_slice},
                                                       _max_loops{max_loops}
                {
                }

                template <typename TF>
                void run(ft frame_time, TF&& f)
                {
                    _loops = 0;
                    _time += frame_time;

                    while(_time >= _time_slice && _loops < _max_loops)
                    {
                        f(_step);
                        _time -= _time_slice;
                        ++_loops;
                    }

                    //  std::cout << "loops: " << _loops << "\n";
                }
            };

            class context
            {
                // TODO:
            public:
                const sz_t _width;
                const sz_t _height;

                unique_window _window;
                unique_glcontext _glcontext;

                SDL_Event _event;

                static_timer _static_timer;

                key_event_handler _on_key_down{null_key_event_handler()};
                key_event_handler _on_key_up{null_key_event_handler()};

                btn_event_handler _on_btn_down{null_btn_event_handler()};
                btn_event_handler _on_btn_up{null_btn_event_handler()};

                update_fn _update_fn{null_update_fn()};
                draw_fn _draw_fn{null_draw_fn()};

                hr_duration _update_duration;
                hr_duration _draw_duration;
                hr_duration _total_duration;
                hr_duration _real_duration;

                double test_now;

                input_state _input_state;

                auto& on_key_up() noexcept;
                auto& on_key_down() noexcept;

                auto& on_btn_up() noexcept;
                auto& on_btn_down() noexcept;

            public:
                auto& update_fn() noexcept;
                auto& draw_fn() noexcept;

            private:
                void run_events();
                void run_update(ft step);
                void run_draw();

                template <typename T>
                auto ms_from_duration(const T& duration) const noexcept;

            public:
                float fps_limit{60.0};

                float ms_limit() const noexcept { return 1000.f / fps_limit; }

                context(const std::string& title, std::size_t width,
                    std::size_t height);

                context(const context&) = delete;
                context& operator=(const context&) = delete;

                context(context&&) = default;
                context& operator=(context&&) = default;

                void run();

                const auto& update_duration() const noexcept;
                const auto& draw_duration() const noexcept;
                const auto& total_duration() const noexcept;
                const auto& real_duration() const noexcept;

                auto update_ms() const noexcept;
                auto draw_ms() const noexcept;
                auto total_ms() const noexcept;
                auto real_ms() const noexcept;

                auto fps() const noexcept;

                auto mouse_x() const noexcept;
                auto mouse_y() const noexcept;
                auto mouse_pos() const noexcept;

                auto key(kkey k) const noexcept;
                auto btn(mbtn b) const noexcept;

                template <typename... Ts>
                auto make_surface(Ts&&... xs) noexcept;

                template <typename... Ts>
                auto make_texture(Ts&&... xs) noexcept;

                // auto make_sprite() noexcept;
                // auto make_sprite(texture& t) noexcept;

                // auto make_ttffont(const std::string& path, sz_t font_size);

                // auto make_ttftext_texture(
                //    ttffont& f, const std::string& s, SDL_Color color);

                // void draw(texture& t, const vec2f& pos) noexcept;
                // void draw(sprite& s) noexcept;

                void title(const std::string& s) noexcept;
            };
        }
    }
}