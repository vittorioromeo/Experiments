// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/elements/surface.hpp>

VRM_SDL_NAMESPACE
{
    auto surface::width() const noexcept { return get().w; }
    auto surface::height() const noexcept { return get().h; }

    auto surface::size() const noexcept { return make_vec(width(), height()); }

    const auto& surface::rect() const noexcept { return _rect; }

    surface::surface(const std::string& path) noexcept
        : base_type{IMG_Load(path.c_str())}
    {
        _rect.x = 0;
        _rect.y = 0;
        _rect.w = width();
        _rect.h = height();
    }

    surface::surface(SDL_Surface * p) noexcept : base_type{p} {}

    auto surface::pixels() const noexcept { return get().pixels; }
    auto surface::format() const noexcept { return get().format; }

    void surface::clear(int r, int g, int b) noexcept
    {
        SDL_FillRect(ptr(), &_rect, SDL_MapRGB(format(), r, g, b));
    }

    void surface::blit(const surface& s, const SDL_Rect& src_rect,
        const SDL_Rect& dest_rect) noexcept
    {
        SDL_BlitSurface(
            s.ptr(), &src_rect, ptr(), &const_cast<SDL_Rect&>(dest_rect));
    }

    void surface::blit(const surface& i, int x, int y) noexcept
    {
        blit(i, i.rect(), SDL_Rect{x, y, i.width(), i.height()});
    }
}
VRM_SDL_NAMESPACE_END