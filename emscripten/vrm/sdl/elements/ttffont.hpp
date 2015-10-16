// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/elements/sdl_element.hpp>

VRM_SDL_NAMESPACE
{
    class ttffont : public impl::sdl_element<TTF_Font>
    {
    public:
        using base_type = impl::sdl_element<TTF_Font>;

        ttffont(const std::string& path, sz_t font_size) noexcept
            : base_type{TTF_OpenFont(path.c_str(), font_size)}
        {
        }
    };
}
VRM_SDL_NAMESPACE_END