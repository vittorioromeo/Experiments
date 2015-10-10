// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <chrono>
#include <vrm/sdl/math.hpp>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/resource.hpp>
#include <vrm/sdl/elements.hpp>
#include <vrm/sdl/context/context.hpp>

namespace vrm
{
    namespace sdl
    {
        auto& context::on_key_up() noexcept { return _on_key_up; }
        auto& context::on_key_down() noexcept { return _on_key_down; }

        auto& context::on_btn_up() noexcept { return _on_btn_up; }
        auto& context::on_btn_down() noexcept { return _on_btn_down; }

        auto& context::update_fn() noexcept { return _update_fn; }
        auto& context::draw_fn() noexcept { return _draw_fn; }

        void context::run_events()
        {
            while(SDL_PollEvent(&_event))
            {
                switch(_event.type)
                {
                    case SDL_KEYDOWN:
                        on_key_down()(
                            static_cast<kkey>(_event.key.keysym.scancode));
                        break;

                    case SDL_KEYUP:
                        on_key_up()(
                            static_cast<kkey>(_event.key.keysym.scancode));
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        on_btn_down()(static_cast<mbtn>(_event.button.button));
                        break;

                    case SDL_MOUSEBUTTONUP:
                        on_btn_up()(static_cast<mbtn>(_event.button.button));
                        break;

                    case SDL_MOUSEMOTION:
                        _input_state.mouse_x(_event.motion.x);
                        _input_state.mouse_y(_event.motion.y);
                        break;

                    case SDL_QUIT: break;
                    case SDL_WINDOWEVENT: break;
                    case SDL_FINGERDOWN: break;
                    case SDL_FINGERUP: break;
                    case SDL_FINGERMOTION: break;
                }
            }
        }

        void context::run_update() { update_fn()(1.f); }
        void context::run_draw()
        {
            //_renderer->clear();
            //_renderer->clear_texture(*_texture, 0, 0, 0, 255);

            //_renderer->target(nullptr);
            //_renderer->draw(*_texture);

            draw_fn()();

            //_renderer->present();
            SDL_GL_SwapWindow(*_window);
        }

        context::context(
            const std::string& title, std::size_t width, std::size_t height)
            : _width{width}, _height{height}, _window{title, width, height},
              _glcontext{*_window}, _renderer{*_window},
              _texture{*_renderer, width, height, SDL_TEXTUREACCESS_STREAMING}
        {
            // glewExperimental = GL_TRUE;
            // glewInit();

            if(TTF_Init() != 0)
            {
                impl::log_sdl_error("ttf_init");
                std::terminate();
            }

            on_key_down() = [this](auto k)
            {
                _input_state.key(k, true);
            };

            on_key_up() = [this](auto k)
            {
                _input_state.key(k, false);
            };

            on_btn_down() = [this](auto b)
            {
                _input_state.btn(b, true);
            };

            on_btn_up() = [this](auto b)
            {
                _input_state.btn(b, false);
            };
        }



        void context::run()
        {
            auto time_dur([](auto&& f)
                {
                    auto ms_start(hr_clock::now());
                    f();

                    return hr_clock::now() - ms_start;
                });

            run_events();

            _update_duration = time_dur([this]
                {
                    run_update();
                });
            _draw_duration = time_dur([this]
                {
                    run_draw();
                });
        }

        // auto& screen() noexcept { return _screen; }
        // const auto& screen() const noexcept { return _screen; }

        const auto& context::update_duration() const noexcept
        {
            return _update_duration;
        }
        const auto& context::draw_duration() const noexcept
        {
            return _draw_duration;
        }
        auto context::total_duration() const noexcept
        {
            return update_duration() + draw_duration();
        }

        auto context::update_ms() const noexcept
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                       update_duration())
                .count();
        }
        auto context::draw_ms() const noexcept
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                       draw_duration())
                .count();
        }
        auto context::total_ms() const noexcept
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                       total_duration())
                .count();
        }

        auto context::fps() const noexcept
        {
            constexpr float seconds_ft_ratio{60.f};
            // return seconds_ft_ratio / total_ms();
            return static_cast<int>(((1.f / total_ms()) * 1000.f));
        }

        auto context::mouse_x() const noexcept
        {
            return _input_state.mouse_x();
        }
        auto context::mouse_y() const noexcept
        {
            return _input_state.mouse_y();
        }
        auto context::mouse_pos() const noexcept
        {
            return _input_state.mouse_pos();
        }

        auto context::key(kkey k) const noexcept { return _input_state.key(k); }
        auto context::btn(mbtn b) const noexcept { return _input_state.btn(b); }

        template <typename T, typename... Ts>
        auto context::make_unique_res(Ts&&... xs)
        {
            using base_type = typename T::base_type;
            using element_type = typename base_type::element_type;
            using unique_type =
                typename impl::unique_sdl_element<element_type>::type;

            return unique_type{FWD(xs)...};
        }

        template <typename... Ts>
        auto context::make_image(Ts&&... xs) noexcept
        {
            return make_unique_res<surface>(FWD(xs)...);
        }

        template <typename... Ts>
        auto context::make_texture(Ts&&... xs) noexcept
        {
            return make_unique_res<texture>(*_renderer, FWD(xs)...);
        }


        auto context::make_sprite() noexcept { return sprite{}; }
        auto context::make_sprite(texture& t) noexcept { return sprite{t}; }

        auto context::make_ttffont(const std::string& path, sz_t font_size)
        {
            return make_unique_res<ttffont>(path, font_size);
        }

        auto context::make_ttftext_texture(
            ttffont& f, const std::string& s, SDL_Color color)
        {
            auto temp(make_unique_res<surface>(
                TTF_RenderText_Blended(f, s.c_str(), color)));
            auto result(make_texture(*temp));
            return result;
        }

        void context::draw(texture& t, const vec2f& pos) noexcept
        {
            _renderer->draw(t, pos);
        }
        void context::draw(sprite& s) noexcept { _renderer->draw(s); }
    }
}