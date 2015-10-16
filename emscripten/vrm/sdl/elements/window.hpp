// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/elements/sdl_element.hpp>

VRM_SDL_NAMESPACE
{
    enum class window_mode
    {
        windowed = 0,
        windowed_fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
        fullscreen = SDL_WINDOW_FULLSCREEN
    };

    class window : public impl::sdl_element<SDL_Window>
    {
    private:
        vec2f _size;
        window_mode _mode;

    public:
        using base_type = impl::sdl_element<SDL_Window>;

        window(const std::string& title, sz_t width, sz_t height) noexcept
            : base_type{SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED, width, height,
                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)},
              _size(width, height)
        {
            mode(window_mode::windowed);
        }

        void mode(window_mode x) noexcept
        {
            SDL_SetWindowFullscreen(*this, static_cast<int>(x));
        }

        const auto& mode() const noexcept { return _mode; }

        void title(const std::string& s) noexcept
        {
            SDL_SetWindowTitle(*this, s.c_str());
        }

        auto size() noexcept
        {
            int rx, ry;
            SDL_GetWindowSize(*this, &rx, &ry);
            return vec2f(rx, ry);
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
}
VRM_SDL_NAMESPACE_END