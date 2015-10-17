// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/gl.hpp>
#include <vrm/sdl/window/window_scale.hpp>

VRM_SDL_NAMESPACE
{
    class window
    {
    public:
        using scale_fn_type = std::function<vec2f(const vec2f&, const vec2f&)>;

    private:
        impl::unique_sdl_window _sdl_window;
        impl::unique_glcontext _glcontext;

        scale_fn_type _scale_fn{window_scale::pixel_perfect()};
        vec2f _original_size;
        float _original_ratio;



    public:
        window(const std::string& title, float width, float height) noexcept
            : _sdl_window{title, width, height},
              _glcontext{*_sdl_window},
              _original_size{width, height},
              _original_ratio{width / height}
        {
        }

        auto& sdl_window() noexcept { return *_sdl_window; }
        const auto& sdl_window() const noexcept { return *_sdl_window; }

        void resize(const vec2f& new_original_size) noexcept
        {
            _original_size = new_original_size;
        }

        void display() noexcept { sdl_window().display(); }

        template <typename TF>
        void scale_fn(TF&& fn) noexcept
        {
            _scale_fn = FWD(fn);
        }

        void mode(window_mode x) noexcept { sdl_window().mode(x); }
        const auto& mode() const noexcept { return sdl_window().mode(); }

        void title(const std::string& s) noexcept { sdl_window().title(s); }

        auto current_size() const noexcept { return sdl_window().size(); }
        const auto& original_size() const noexcept { return _original_size; }
        const auto& original_ratio() const noexcept { return _original_ratio; }

        auto scaled_size() const noexcept
        {
            return _scale_fn(_original_size, current_size());
        }

        auto scaling_factor() const noexcept
        {
            return scaled_size().x / _original_size.x;
        }

        auto margin() const noexcept
        {
            return impl::ratio_scale_margin(scaled_size(), current_size());
        }

        void use_background() noexcept
        {
            sdl_window().scissor_and_viewport({0.f, 0.f}, current_size());
        }

        void use_foreground() noexcept
        {
            sdl_window().scissor_and_viewport(margin(), scaled_size());
        }

        void clear(const vec4f& color) noexcept { sdl_window().clear(color); }

        void use_and_clear_background(const vec4f& color) noexcept
        {
            use_background();
            clear(color);
        }

        void use_and_clear_foreground(const vec4f& color) noexcept
        {
            use_foreground();
            clear(color);
        }
    };
}
VRM_SDL_NAMESPACE_END

// TODO:
// comment everything
// think about "screen" and "camera"
// let user choose stuff
