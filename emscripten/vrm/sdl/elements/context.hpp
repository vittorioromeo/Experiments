// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/math.hpp>
#include <vrm/sdl/common.hpp>

namespace vrm
{
    namespace sdl
    {
        class context
        {
        private: 
            const std::size_t _width;
            const std::size_t _height;

            window _window;
            glcontext _glcontext;
            renderer _renderer;
            texture _texture;

            SDL_Event _event;

            key_event_handler _on_key_down{impl::null_key_event_handler()};
            key_event_handler _on_key_up{impl::null_key_event_handler()};

            btn_event_handler _on_btn_down{impl::null_btn_event_handler()};
            btn_event_handler _on_btn_up{impl::null_btn_event_handler()};

            update_fn _update_fn{impl::null_update_fn()};
            draw_fn _draw_fn{impl::null_draw_fn()};

            impl::input_state _input_state;

            auto& on_key_up() noexcept { return _on_key_up; }
            auto& on_key_down() noexcept { return _on_key_down; }

            auto& on_btn_up() noexcept { return _on_btn_up; }
            auto& on_btn_down() noexcept { return _on_btn_down; }

        public:
            auto& update_fn() noexcept { return _update_fn; }
            auto& draw_fn() noexcept { return _draw_fn; }

        private:
            void run_events()
            {
                while(SDL_PollEvent(&_event))
                {
                    switch(_event.type)
                    {
                        case SDL_KEYDOWN:
                            on_key_down()(_event.key.keysym.sym);
                            break;

                        case SDL_KEYUP:
                            on_key_up()(_event.key.keysym.sym);
                            break;

                        case SDL_MOUSEBUTTONDOWN:
                            on_btn_down()(_event.button.button);
                            break;

                        case SDL_MOUSEBUTTONUP:
                            on_btn_up()(_event.button.button);
                            break;

                        case SDL_MOUSEMOTION:
                            _input_state.mouse_x(_event.motion.x);
                            _input_state.mouse_y(_event.motion.y);
                            break;
                    }
                }
            }

            void run_update() { update_fn()(1.f); }
            void run_draw()
            {
                _renderer.clear();
                _renderer.clear_texture(_texture, 0, 0, 0, 255);

                _renderer.target(nullptr);
                _renderer.draw(_texture);

                draw_fn()();

                _renderer.present();
            }

        public:
            context(std::size_t width, std::size_t height)
                : _width{width}, _height{height}, _window{width, height},
                  _glcontext{_window}, _renderer{_window},
                  _texture{_renderer, width, height}
            {
                if(TTF_Init() != 0)
                {
                    std::terminate();
                }

                // SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

                //            _screen = surface{SDL_SetVideoMode(_width,
                //            _height,
                //            32, SDL_SWSURFACE)};


                on_key_down() = [this](auto k)
                {
                    _input_state.key(k) = true;
                };

                on_key_up() = [this](auto k)
                {
                    _input_state.key(k) = false;
                };

                on_btn_down() = [this](auto b)
                {
                    _input_state.btn(b) = true;
                };

                on_btn_up() = [this](auto b)
                {
                    _input_state.btn(b) = false;
                };
            }

            context(const context&) = delete;
            context& operator=(const context&) = delete;

            context(context&&) = default;
            context& operator=(context&&) = default;


            void run()
            {
                run_events();
                run_update();
                run_draw();
            }

            // auto& screen() noexcept { return _screen; }
            // const auto& screen() const noexcept { return _screen; }

            auto mouse_x() const noexcept { return _input_state.mouse_x(); }
            auto mouse_y() const noexcept { return _input_state.mouse_y(); }
            auto mouse_pos() const noexcept { return _input_state.mouse_pos(); }

            auto key(key_code c) const { return _input_state.key(c); }
            auto btn(mouse_btn b) const { return _input_state.btn(b); }

            auto make_texture() noexcept { return texture{}; }
            auto make_texture(const std::string& path) noexcept
            {
                texture result;
                _renderer.load_texture(result, path);
                return result;
            }

            auto make_sprite() noexcept { return sprite{}; }
            auto make_sprite(texture& t) noexcept { return sprite{t}; }

            auto make_ttffont(const std::string& path, sz_t font_size)
            {
                return ttffont{path, font_size};
            }

            auto make_ttftext_texture(
                ttffont& f, const std::string& s, SDL_Color color)
            {
                surface temp{TTF_RenderText_Blended(f, s.c_str(), color)};
                return texture{_renderer, temp};
            }

            void draw(texture& t, const vec2f& pos) { _renderer.draw(t, pos); }
            void draw(sprite& s) { _renderer.draw(s); }
        };
    }
}