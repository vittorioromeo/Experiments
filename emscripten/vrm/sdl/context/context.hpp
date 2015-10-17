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
#include <vrm/sdl/context/static_timer.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        // TODO:
        template <typename TSettings>
        class non_interpolated_engine
        {
            // TODO:
        public:
            using settings_type = TSettings;
            using timer_type = typename settings_type::timer_type;
            using state_type = typename settings_type::state_type;
            using update_fn_type = typename settings_type::update_fn_type;
            using draw_fn_type = typename settings_type::draw_fn_type;
            using interpolate_fn_type =
                typename settings_type::interpolate_fn_type;

            timer_type* _timer;

            state_type _current_state;

            update_fn_type _update_fn{settings_type::null_update_fn()};
            draw_fn_type _draw_fn{settings_type::null_draw_fn()};
            interpolate_fn_type _interpolate_fn{
                settings_type::null_interpolate_fn()};

        public:
            auto& update_fn() noexcept;
            auto& draw_fn() noexcept;
            auto& interpolate_fn() noexcept;

        public:
            void run_update(ft step);
            void run_draw();

        public:
            non_interpolated_engine() = default;

            non_interpolated_engine(const non_interpolated_engine&) = delete;
            non_interpolated_engine& operator=(
                const non_interpolated_engine&) = delete;

            non_interpolated_engine(non_interpolated_engine&&) = default;
            non_interpolated_engine& operator=(
                non_interpolated_engine&&) = default;

            auto& current_state() noexcept { return _current_state; }
            const auto& current_state() const noexcept
            {
                return _current_state;
            }
        };

        template <typename TSettings>
        class interpolated_engine
        {
            // TODO:
        public:
            using settings_type = TSettings;
            using timer_type = typename settings_type::timer_type;
            using state_type = typename settings_type::state_type;
            using update_fn_type = typename settings_type::update_fn_type;
            using draw_fn_type = typename settings_type::draw_fn_type;
            using interpolate_fn_type =
                typename settings_type::interpolate_fn_type;

            timer_type* _timer;

            state_type _prev_state;
            state_type _current_state;
            state_type _interpolated_state;

            update_fn_type _update_fn{settings_type::null_update_fn()};
            draw_fn_type _draw_fn{settings_type::null_draw_fn()};
            interpolate_fn_type _interpolate_fn{
                settings_type::null_interpolate_fn()};

        public:
            auto& update_fn() noexcept;
            auto& draw_fn() noexcept;
            auto& interpolate_fn() noexcept;

        public:
            void run_update(ft step);
            void run_draw();

        public:
            interpolated_engine() = default;

            interpolated_engine(const interpolated_engine&) = delete;
            interpolated_engine& operator=(const interpolated_engine&) = delete;

            interpolated_engine(interpolated_engine&&) = default;
            interpolated_engine& operator=(interpolated_engine&&) = default;

            auto& current_state() noexcept { return _current_state; }
            const auto& current_state() const noexcept
            {
                return _current_state;
            }
        };

        template <typename TSettings>
        class context
        {
            // TODO:
        public:
            using settings_type = TSettings;
            using timer_type = typename settings_type::timer_type;
            using engine_type = typename settings_type::engine_type;

            engine_type* _engine;
            window& _window;

            SDL_Event _event;

            key_event_handler _on_key_down{null_key_event_handler()};
            key_event_handler _on_key_up{null_key_event_handler()};

            btn_event_handler _on_btn_down{null_btn_event_handler()};
            btn_event_handler _on_btn_up{null_btn_event_handler()};

            hr_duration _update_duration;
            hr_duration _draw_duration;
            hr_duration _total_duration;
            hr_duration _real_duration;

            input_state _input_state;

            auto& on_key_up() noexcept;
            auto& on_key_down() noexcept;

            auto& on_btn_up() noexcept;
            auto& on_btn_down() noexcept;

            auto& timer() noexcept;
            const auto& timer() const noexcept;

        public:
            void run_window_events() noexcept;
            void run_events() noexcept;

            void limit_fps_if_necessary() const noexcept;

            template <typename T>
            auto ms_from_duration(const T& duration) const noexcept;

            auto& window() noexcept { return _window; }
            const auto& window() const noexcept { return _window; }

        public:
            float fps_limit{144.0};

            float ms_limit() const noexcept { return 1000.f / fps_limit; }

            context(class window& window);

            context(const context&) = delete;
            context& operator=(const context&) = delete;

            context(context&&) = default;
            context& operator=(context&&) = default;

            void run();
            void clear(const vec4f& color) noexcept;

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

            void title(const std::string& s) noexcept;
        };
    }
}
VRM_SDL_NAMESPACE_END