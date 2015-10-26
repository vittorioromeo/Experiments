// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/elements/sdl_element.hpp>
#include <vrm/sdl/resource.hpp>

VRM_SDL_NAMESPACE
{
    class sdl_window : public impl::sdl_element<SDL_Window>
    {
        friend struct ::vrm::sdl::impl::sdl_window_deleter;

    public:
        using no_params_fn = delegate<void()>;
        using resized_fn = delegate<void(const vec2i&)>;
        using focus_fn = delegate<void(window_focus_change)>;

    private:
        window_mode _mode;
        vec2f _size;
        bool _open;
        bool _in_focus;

        // Global event delegate handles.
        delegate_handle _dh_resized;
        delegate_handle _dh_focus_changed;
        delegate_handle _dh_closed;

        // Delegates.
        resized_fn _on_resized;
        focus_fn _on_focus_changed;
        no_params_fn _on_closed;

        static constexpr auto flags =
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

        auto this_id() const noexcept { return SDL_GetWindowID(this->ptr()); }

        void bind_events() noexcept
        {
            // TODO: `unique_event_handle`

            auto& em(get_global_event_manager());

            _dh_resized = em.on_window_resized() +=
                [this](auto id, const auto& new_size)
            {
                if(id != this_id()) return;

                _size = new_size;
                _on_resized(new_size);
            };

            _dh_focus_changed = em.on_window_focus_changed() +=
                [this](auto id, auto change)
            {
                if(id != this_id()) return;

                _in_focus = change == window_focus_change::gained;
                _on_focus_changed(change);
            };

            _dh_closed = em.on_window_closed() += [this](auto id)
            {
                if(id != this_id()) return;

                _open = false;
                _on_closed();
            };
        }

        void unbind_events()
        {
            auto& em(get_global_event_manager());

            em.on_window_resized() -= _dh_closed;
            em.on_window_focus_changed() -= _dh_focus_changed;
            em.on_window_closed() -= _dh_resized;
        }

    public:
        using base_type = impl::sdl_element<SDL_Window>;

        sdl_window(const std::string& title, const vec2u& size) noexcept
            : base_type{SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED, size.x, size.y, flags)},
              _size(size),
              _open{true},
              _in_focus{true}
        {
            mode(window_mode::windowed);
            bind_events();
        }

        const auto& mode() const noexcept { return _mode; }
        const auto& size() const noexcept { return _size; }
        const auto& open() const noexcept { return _open; }
        const auto& in_focus() const noexcept { return _in_focus; }

        auto& on_resized() noexcept { return _on_resized; }
        auto& on_focus_changed() noexcept { return _on_focus_changed; }
        auto& on_closed() noexcept { return _on_closed; }

        void display() noexcept { SDL_GL_SwapWindow(*this); }

        void mode(window_mode x) noexcept
        {
            SDL_SetWindowFullscreen(*this, vrmc::from_enum(x));
        }

        void title(const std::string& s) noexcept
        {
            SDL_SetWindowTitle(*this, s.c_str());
        }

        void scissor(const vec2f& position, const vec2f& size) const noexcept
        {
            glScissor(position.x, position.y, size.x, size.y);
        }

        void viewport(const vec2f& position, const vec2f& size) const noexcept
        {
            glViewport(position.x, position.y, size.x, size.y);
        }

        void scissor_and_viewport(
            const vec2f& position, const vec2f& size) const noexcept
        {
            scissor(position, size);
            viewport(position, size);
        }

        void clear(const vec4f& color) noexcept
        {
            glClearColor(color.x, color.y, color.z, color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                    GL_STENCIL_BUFFER_BIT);
        }
    };

    namespace impl
    {
        struct sdl_window_deleter
        {
            void operator()(sdl_window& p) noexcept
            {
                SDL_DestroyWindow(p.ptr());
                p.unbind_events();
            }
        };

        using unique_sdl_window =
            unique_resource<sdl_window, sdl_window_deleter>;
    }
}
VRM_SDL_NAMESPACE_END