// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/resource.hpp>
#include <vrm/sdl/utils.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        class sdl_event_manager
        {
        public:
            using window_id = int;

            using key_fn = delegate<void(kkey)>;
            using btn_fn = delegate<void(mbtn)>;
            using mouse_motion_fn = delegate<void(const vec2f&)>;
            using window_fn = delegate<void(window_id)>;
            using window_resized_fn = delegate<void(window_id, const vec2i&)>;
            using window_focus_fn =
                delegate<void(window_id, window_focus_change)>;

        private:
            SDL_Event _event;

            key_fn _on_key_down, _on_key_up;
            btn_fn _on_btn_down, _on_btn_up;
            mouse_motion_fn _on_mouse_motion;
            window_resized_fn _on_window_resized;
            window_focus_fn _on_window_focus_changed;
            window_fn _on_window_closed;

        public:
            auto& on_key_down() noexcept { return _on_key_down; }
            auto& on_key_up() noexcept { return _on_key_up; }

            auto& on_btn_down() noexcept { return _on_btn_down; }
            auto& on_btn_up() noexcept { return _on_btn_up; }

            auto& on_mouse_motion() noexcept { return _on_mouse_motion; }

            auto& on_window_resized() noexcept { return _on_window_resized; }
            auto& on_window_focus_changed() noexcept
            {
                return _on_window_focus_changed;
            }
            auto& on_window_closed() noexcept { return _on_window_closed; }

        private:
            void run_window_events() noexcept
            {
                auto id(_event.window.windowID);
                auto data1(_event.window.data1);
                auto data2(_event.window.data2);

                switch(_event.window.event)
                {
                    case SDL_WINDOWEVENT_SHOWN: break;
                    case SDL_WINDOWEVENT_HIDDEN: break;
                    case SDL_WINDOWEVENT_EXPOSED: break;
                    case SDL_WINDOWEVENT_MOVED: break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED: break;
                    case SDL_WINDOWEVENT_MINIMIZED: break;
                    case SDL_WINDOWEVENT_MAXIMIZED: break;
                    case SDL_WINDOWEVENT_RESTORED: break;
                    case SDL_WINDOWEVENT_ENTER: break;
                    case SDL_WINDOWEVENT_LEAVE: break;

                    case SDL_WINDOWEVENT_RESIZED:
                        on_window_resized()(id, make_vec(data1, data2));
                        break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        on_window_focus_changed()(
                            id, window_focus_change::gained);
                        break;

                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        on_window_focus_changed()(
                            id, window_focus_change::lost);
                        break;

                    case SDL_WINDOWEVENT_CLOSE: on_window_closed()(id); break;
                }
            }

            void run_events() noexcept
            {
                switch(_event.type)
                {
                    case SDL_KEYDOWN:
                        on_key_down()(
                            to_enum<kkey>(_event.key.keysym.scancode));
                        break;

                    case SDL_KEYUP:
                        on_key_up()(to_enum<kkey>(_event.key.keysym.scancode));
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        on_btn_down()(to_enum<mbtn>(_event.button.button));
                        break;

                    case SDL_MOUSEBUTTONUP:
                        on_btn_up()(to_enum<mbtn>(_event.button.button));
                        break;

                    case SDL_MOUSEMOTION:
                        on_mouse_motion()(
                            vec2f(_event.motion.x, _event.motion.y));
                        break;

                    case SDL_WINDOWEVENT: run_window_events(); break;

                    case SDL_FINGERDOWN: break;
                    case SDL_FINGERUP: break;
                    case SDL_FINGERMOTION: break;

                    case SDL_QUIT: std::terminate(); break;
                }
            }

        public:
            void run() noexcept
            {
                while(SDL_PollEvent(&_event))
                {
                    run_events();
                }
            }
        };

        struct sdl_window_deleter;
    }

    VRM_SDL_ALWAYS_INLINE auto& get_global_event_manager() noexcept
    {
        static impl::sdl_event_manager result;
        return result;
    }
}
VRM_SDL_NAMESPACE_END