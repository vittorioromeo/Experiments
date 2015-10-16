// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/math.hpp>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/elements/sdl_element.hpp>

VRM_SDL_NAMESPACE
{
    class surface : public impl::sdl_element<SDL_Surface>
    {
    private:
        SDL_Rect _rect;

    public:
        using base_type = impl::sdl_element<SDL_Surface>;

        auto width() const noexcept;
        auto height() const noexcept;
        auto size() const noexcept;
        const auto& rect() const noexcept;

        surface(const std::string& path) noexcept;
        surface(SDL_Surface* p) noexcept;


        auto pixels() const noexcept;
        auto format() const noexcept;

        void clear(int r, int g, int b) noexcept;

        void blit(const surface& s, const SDL_Rect& src_rect,
            const SDL_Rect& dest_rect) noexcept;

        void blit(const surface& i, int x, int y) noexcept;
    };
}
VRM_SDL_NAMESPACE_END